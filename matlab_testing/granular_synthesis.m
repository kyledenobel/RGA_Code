% Attempt at granular synthesis for pitch shifting
rho = 1;

% ============= begin implementation =============  %

% read audio sample
[x, Fs] = audioread("G-scale.wav");
samples = 6*Fs; % 3 seconds of samples
x = x(1:samples, :);

% let S = the stride length  (grain period = 10ms)
T = 0.02;
S = floor(Fs*T);

% generate output
y = zeros(numel(x),1);

gamma = 1/(1+(rho*S)/(S-1));

offset = 0;
number_of_times_buffer_filled = 0;

for n = 0:(numel(x))-1
    % only fill buffers when we need to
    if mod(n, S) == 0
        % generate buffers
        a = zeros(1, 2*S);
        b = zeros(1, 2*S);
        % fill buffers
        for k = 0:((rho+1)*S)-1
            if floor(k*gamma + 1 + (S*number_of_times_buffer_filled)) < numel(x) + 1
                a(k+1) = x(floor(k*gamma + 1 + (S*number_of_times_buffer_filled)));
            else
                a(k+1) = 0;
            end
            if floor(k*gamma + 1 + S + (S*number_of_times_buffer_filled)) < numel(x) + 1
                b(k+1) = x(floor(k*gamma + 1 + S + (S*number_of_times_buffer_filled)));
            else
                b(k+1) = 0;
            end
        end
        number_of_times_buffer_filled = number_of_times_buffer_filled + 1;

        offset = rand()*30 - 15;
    end

    % calculate window
    m = mod(n,S);
    if rho == 0
        w = 1;
    else
        %w = myLinearWindow(m, S, rho, offset);
        w = mySineWindow(m, S, rho, offset);
    end

    % do computation of y
    y(n+1) = w*(b(mod(n, S) + 1)) + (1 - w)*(a(S + mod(n, S) + 1));
end


% play output

player = audioplayer(y, Fs);
play(player)

figure(1)
subplot(2,1,1)
plot(x)
subplot(2,1,2)
plot(y)


X = fft(x, int32(2*Fs));
X = fftshift(X);

Y = fft(y, int32(2*Fs));
Y = fftshift(Y);

x_axis = linspace(-Fs, Fs, int32(2*Fs))./2;
figure(2)
subplot(2,1,1)
plot(x_axis, abs(X))
xlim([-2000, 2000])
subplot(2,1,2)
plot(x_axis, abs(Y))
xlim([-2000, 2000])



function v = myLinearWindow(m, S, rho, offset)
    if m < (S*rho + offset)
        v = m/(S*rho);
    else
        v = 1;
    end
end


function v = mySineWindow(m, S, rho, offset)
    v = sin(m*2*pi*(1/(2*(S*(1+rho) + offset))));
    if v < 0
        v = 0;
    end
end