StringFreq = [82, 110, 147, 196, 247, 330];
freq_offset = 0.05;
freq_offset = 10;

dsf = 32;
N = 128;

[x, fs] = audioread("AcousticGuitar.wav");


% Downsample x
x = x(1:dsf:length(x));
fs = fs/dsf;

count = floor(length(x) / N);
x = x(1:count*N);
x = reshape(x, [N, count]);

clf
intensity_window = [0.2 0.6 0.22];

intensities = [];
ratios = [];
for Selected = 1:6
    %sampled = ((-1:1:1) * freq_offset + 1) * StringFreq(Selected);
    sampled = ((-1:1:1) * freq_offset) + StringFreq(Selected);
    freq_indices = (N * (sampled) / fs) + 1;
    
    res = abs(goertzel(x, freq_indices, 1));

    intensities(Selected, :) = intensity_window * res;

    ratio = res(1, :) - res(3, :);
    ratios(Selected, :) = ratio;

    %plot(input_frequencies, new_res.' - 100, '--')
    %plot(1:length(x), abs(new_res.') - 100, '--')
end

correction_factor = [5; 3; 1.4; 1; 0.75; 0.5];
intensities = correction_factor .* intensities;

max_intensity = max(intensities, [], 1);
intensity_ratio = intensities ./ max_intensity;
[row, col] = find(intensity_ratio == 1);

second_place = intensity_ratio;
second_place(second_place == 1) = 0;
second_place = max(second_place, [], 1);


true_ratio = ratios(find(intensity_ratio == 1));

row(max_intensity < 1.5) = 0;
row(second_place > 0.5) = 0;

true_ratio(max_intensity < 1.5) = 0;
true_ratio(second_place > 0.5) = 0;
%second_place(second_place > 0.5) = 1;

clf
subplot(3, 1, 1);
title('Intensities')
xlabel('Time (seconds)')
ylabel('High/Low Ratio')
plot((1:N:N*count)/fs, intensities, '-');

subplot(3, 1, 2);
title('String Detection')
xlabel('Time (seconds)')
ylabel('Detected String')
plot((1:N:N*count)/fs, row, '-');



subplot(3, 1, 3);
title('Tuning Ratio')
xlabel('Time (seconds)')
ylabel('Low-High Ratio')
plot((1:N:N*count)/fs, true_ratio, '--');


