StringFreq = [82, 110, 147, 196, 247, 330];
freq_offset = 0.03;


clf
hold on
for Selected = 1:6
    sampled = ((-2:4:2) * freq_offset + 1) * StringFreq(Selected);
    
    input_frequencies = StringFreq(Selected)*0.8:0.25:StringFreq(Selected)*1.2;
    
    fs = 96000/32;
    N = 128;
    
    res = GoertzelRound(sampled, input_frequencies, fs, N);

    %new_res = max([res(1,:)-res(3,:); res(5,:) - res(3,:)], [], 1);
    %new_res = max([res(1,:)-res(3,:); res(5,:) - res(3,:)], [], 1);
    new_res = res(1, :) - res(2, :);
    
    plot(input_frequencies, res.');

    %plot(input_frequencies, new_res.' - 100, '--')
    plot(input_frequencies, abs(new_res.') - 100, '--')

end

