StringFreq = [82, 110, 147, 196, 247, 330];

target_relative_amp = 0.9;
low_relative_amp = 0.2;
cutoff = 5e-3;

[x, fs] = audioread("AcousticGuitar.wav");

max_freq = 600;
min_sample_gap = floor(fs/max_freq);
max_window = ceil(fs/40);
x = x(1:(floor(length(x)/max_window)*max_window));

N = length(x);

max_val = max(reshape(x, max_window, []), [], 1);
min_val = min(reshape(x, max_window, []), [], 1);

peaks = zeros(N, 1) == 1;
reached_mid = false;
reached_high = true;
first_delta = true;
wavelengths = [];
wavelengths2 = [];
start = true;



lr_size = 5;
lr_max_time = fs * lr_size / 40;
last_read = zeros(1, lr_size) - lr_max_time;

dw_size = 15;
dw_cutoff = 5;
for i= max_window:N
    % restart at beginning of each window
    if (mod(i-1, max_window) == 0)
        start = true;
    end
    % What are we using as reference for what our peaks should be
    max_i = max(floor(i/max_window) - 1, 1);
    high_target = max_val(max_i) * target_relative_amp;
    low_target = min_val(max_i) * target_relative_amp;
    mid_target = max_val(max_i) + abs(min_val(max_i)) * low_relative_amp / 2;

    % Get sample
    sample = x(i);

    if (high_target < cutoff)
        if (start)
            last_read = [i last_read(2:end)];
            wavelengths(1:4, end+1) = [96000; 96000; max_i; i];
            wavelengths2(1:4, end+1) = [96000; 96000; max_i; i];
        end
        continue
    end

    reached_high = sample > high_target;
    if (reached_high && reached_mid)
        reached_mid = false;
        peaks(i) = true;
        if (start) 
            last_peak = i;
            start = false;
        else
            delta = i - last_peak;
            if (delta >= min_sample_gap)
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
                
                last_peak = i;
                last_read = [i last_read(1:end-1)];
                wavelengths(1:4, end+1) = [delta; av_delta; max_i; i];
                if (i - last_read(end) <= lr_max_time)
                    wavelengths2(1:4, end+1) = [delta; av_delta; max_i; i];
                end
            end
        end
    end
    if (sample < low_target)%(abs(sample) < mid_target)
        reached_mid = true;
    end
end

slice = 1:20:N;

clf;
hold on
freq = fs./wavelengths(2, :);
freq(freq > 400) = 0;
freq(freq < 20) = 0;
%   plot(wavelengths(4, :), fs./wavelengths(1, :))
plot(wavelengths(4, :), freq)
freq = fs./wavelengths2(2, :);
freq(freq > 400) = 0;
freq(freq < 20) = 0;
plot(wavelengths2(4, :), freq)

x = x(slice);
plot(slice, 100*(x))
%%
t = zeros(1, N);
t(peaks) = 1;


t = t(slice);

plot(slice, t)

