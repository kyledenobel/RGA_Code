% Version 3 of Granular Synthesis


rho = 0.3;


% ========== begin implementation ========== %
% read audio sample
[x, Fs] = audioread("G-scale.wav");
samples = 6*Fs; % 3 seconds of samples
x = x(1:samples, :);

%{
Fs = 10000;
tt = 0:1/Fs:2;
f = 300;
x = cos(2*pi*tt*f);
%}

% let S = the stride length  (grain period = 25ms)
T = 0.04;
S = floor(Fs*T);

% generate output
y = zeros(numel(x),1);

% parameters
gamma = 1/(1+rho);

% calculate window
w = zeros(1, floor(S/gamma));
for k = 1:floor(S/gamma)
    if k < rho*S
        w(k) = 1/(rho*S)*k;
    elseif k <= S
        w(k) = 1;
    else
        w(k) = (1/(S-(S/gamma)))*k - 1/(gamma-1);
    end
end

% initialize beta
beta = 1;

% starting point of buffer
sigma = 1;

% initialize buffer
z = zeros(1, 2*S);

input_index = mod(sigma+S, S) + 1;

% begin computation loop
for n = 1:numel(x)
    % for each pass, we must do the following
    % 1) take in new value for our circular buffer
    % 2) calculate y
    % 3) increment beta, and check we don't pass threshold
    %       If threshold is passed, reset beta and change starting point of
    %       buffer
    
    z(input_index) = x(n);
    if(beta <= S)
        y(n) = ((1 - (beta*gamma - floor(beta*gamma)))*z(mod(floor(beta*gamma)+sigma, 2*S) + 1) + (beta*gamma - floor(beta*gamma))*z(mod(ceil(beta*gamma)+sigma, 2*S)+1))*w(beta);
    else
        y(n) = ((1 - (beta*gamma - floor(beta*gamma)))*z(mod(floor(beta*gamma)+sigma, 2*S) + 1) + (beta*gamma - floor(beta*gamma))*z(mod(ceil(beta*gamma)+sigma, 2*S)+1))*w(beta) + ...
                ((1 - (beta*gamma - floor(beta*gamma)))*z(mod(floor((beta-S)*gamma)+S+sigma, 2*S)+1) + (beta*gamma-floor(beta*gamma))*z(mod(ceil((beta-S)*gamma)+S+sigma, 2*S)+1))*w(beta-S);
    end

    % increment beta
    beta = beta + 1;
    if beta > S/gamma
        beta = floor(rho*S);
        sigma = mod(sigma + S, 2*S);
    end

    % increment the input_index
    input_index = input_index + 1;
    if input_index > 2*S
        input_index = 1;
    end
end


% ========== analysis ========== %

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
i = 1:3000;
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