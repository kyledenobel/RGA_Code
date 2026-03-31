function res=GoertzelRound(sampled, input_frequencies, fs, N)    
    freq_indices = (N * (sampled) / fs) + 1;
    
    n = 0:N-1;
    t = n/fs;
    
    offset = 0; % for imaginary number testing
    
    x = cos(2 * pi .* (t.' .* input_frequencies) + offset);
    
    res = abs(goertzel(x, freq_indices, 1));
end