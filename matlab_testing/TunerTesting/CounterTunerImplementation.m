StringFreq = [82, 110, 147, 196, 247, 330];

% Get cutoff thresholds for each one
NoteCutoffs = movmean(StringFreq, 2);
NoteCutoffs = NoteCutoffs(2:end); % who needs the first
NoteBounds = zeros(2, length(StringFreq));
NoteBounds(1, 2:end) = NoteCutoffs;
NoteBounds(2, 1:end-1) = NoteCutoffs;
NoteBounds(1, 1) = 60;
NoteBounds(2, end) = 380;

target_relative_amp = 0.9;
low_relative_amp = 0.2;
cutoff = 5e-3;

[x, fs] = audioread("ElectricNotNoisy.wav");
%x = x(1:2:end);
%fs = fs/2;

max_freq = 600;
min_sample_gap = floor(fs/max_freq);
max_window = ceil(fs/40);
x = x(1:(floor(length(x)/max_window)*max_window));

N = length(x);

max_val = max(reshape(x, max_window, []), [], 1);
min_val = min(reshape(x, max_window, []), [], 1);

peaks = zeros(N, 1) == 1;
reached_low = false;
reached_high = true;
first_delta = true;
wavelengths = [];
start = true;

history = zeros(1, max_window);
curr_max = 0;
curr_min = 0;

last_peak = 1;

dw_size = 5;
dw_cutoff = 2;

rolling_av = 0;
rolling_m = 0.4;

for i = 1:N
    h_i = mod(i-1, max_window) + 1;
    % restart at beginning of each window
    if (h_i == 1)
        start = true;
        high_target = curr_max * target_relative_amp;
        low_target = curr_min * target_relative_amp;
        mid_target = abs(curr_max) + abs(curr_min) * low_relative_amp / 2;

        curr_max = 0;
        curr_min = 0;
    end
    

    % Get sample from history
    sample = history(h_i);

    if (high_target < cutoff)
        if (start)
            rolling_av = (1 - rolling_m) * rolling_av + rolling_m * 96000;
            wavelengths(1:4, end+1) = [96000; 0; rolling_av; i];    
            start = false;
        end
    else
        reached_high = sample > high_target;
        if (reached_high && reached_low)
            reached_low = false;
            peaks(i) = true;
            if (start) 
                last_peak = i;
                start = false;
            else
                delta = i - last_peak;
                if (delta >= min_sample_gap)
                    last_peak = i;

                    if (first_delta)
                        first_delta = false;
                        delta_hist = zeros(1, dw_size) + delta;
                    else
                        delta_hist(2:dw_size) = delta_hist(1:dw_size-1);
                        delta_hist(1) = delta;
                    end
    
                    av_delta = sort(delta_hist);
                    av_delta = av_delta(1+dw_cutoff:dw_size-dw_cutoff);
                    av_delta = mean(av_delta);
                    
                    av_delta = max(min(av_delta, fs/20), fs/max_freq);

                    rolling_av = (1 - rolling_m) * rolling_av + rolling_m * av_delta;
                    
                    wavelengths(1:4, end+1) = [delta; fs/av_delta; fs/rolling_av; i];
                end
            end
        end
        if (sample < low_target)%(abs(sample) < mid_target)
            reached_low = true;
        end
    end
    
    % Deal with history / setting curr max/min
    history(h_i) = x(i);

    curr_max = max(curr_max, x(i));
    curr_min = min(curr_min, x(i));
end

slice = 1:20:N;

clf;
hold on
freq = wavelengths(2, :);
freq(freq > 400) = 0;
freq(freq < 20) = 0;
plot(wavelengths(4, :)/fs, freq, DisplayName="Frequency Detected")
xlabel("Time")
ylabel("Frequency / Signal")

%freq = wavelengths(3, :);
%freq(freq > 400) = 0;
%freq(freq < 20) = 0;
%plot(wavelengths(4, :)/fs, freq)

x = x(slice);
plot(slice/fs, 100*(x), DisplayName="Audio Signal")

