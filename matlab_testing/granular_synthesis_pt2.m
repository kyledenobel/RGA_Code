% Version 2 of Granular Synthesis
% Improvements
% 1) Interpolation between values in buffers
% 2) 

rho = 0.3;


% ========== begin implementation ========== %
% read audio sample
[x, Fs] = audioread("G-scale.wav");
samples = 6*Fs; % 3 seconds of samples
x = x(1:samples, :);

% let S = the stride length  (grain period = 25ms)
T = 0.025;
S = floor(Fs*T);

% generate output
y = zeros(numel(x),1);

% parameters
gamma = 1/(1+rho);

a = zeros(1, 2*S);
b = zeros(1, 2*S);
offset = rand()*30 - 15;

for i = 1:numel(x)

    % this if statement is to ensure we only take action on data once
    if mod(i-1, S) == 0
        % setup buffers
        a = zeros(1, 2*S);
        b = zeros(1, 2*S);
        % ensure that we will not go over our limit of x
        if i+(2*S/gamma) < numel(x)
            % fill buffers
            for k = 1:floor(S/gamma)
                a(k) = (1-(k*gamma-floor(k*gamma)))*x(floor(k*gamma) + i) + (k*gamma-floor(k*gamma))*x(ceil(k*gamma) + i);
                b(k) = (1-(k*gamma-floor(k*gamma)))*x(floor(k*gamma) + i + S) + (k*gamma-floor(k*gamma))*x(ceil(k*gamma) + i + S);
            end
        end
        offset = rand()*30 - 15;
    end

    % stitch buffers together
    % calculate window
    m = mod(i,S);
    if rho == 0
        w = 1;
    else
        %w = myLinearWindow(m, S, rho, offset);
        w = mySineWindow(m, S, rho, 0);
    end

    % do computation of y
    y(i) = w*(b(mod(i, S) + 1)) + (1 - w)*(a(S + mod(i, S) + 1));

end

% play output
player = audioplayer(y, Fs);
play(player)

% plot full input and output
figure(1)
subplot(2,1,1)
plot(x);
subplot(2,1,2)
plot(y);

% plot just one note
i = 76615:95063;
i = 76615:80000;
xx = x(i);
yy = y(i);
figure(2)
subplot(2,1,1)
plot(xx);
subplot(2,1,2)
plot(yy);

% take fft of just one note
X = fft(xx, int32(2*Fs));
X = fftshift(X);

Y = fft(yy, int32(2*Fs));
Y = fftshift(Y);

x_axis = linspace(-Fs, Fs, int32(2*Fs))./2;
figure(3)
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


