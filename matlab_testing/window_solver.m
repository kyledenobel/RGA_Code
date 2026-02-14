% window solver

syms rho S g

% ========== for cubic window ========== %
disp("================ Cubic: k < rho*S ================")
W = [1, 1, 1, 1, 0;
     3, 2, 1, 0, 0;
     (rho*S)^3, (rho*S)^2, rho*S, 1, 1;
     3*(rho*S)^2, 2*rho*S, 1, 0, 0];

W = rref(W);
pretty(W)
disp("================ Cubic: k > S ====================")
W = [S^3, S^2, S, 1, 1;
     (S/g)^3, (S/g)^2, (S/g), 1, 0;
     3*S^2, 2*S, 1, 0, 0;
     3*(S/g)^2, 2*(S/g), 1, 0, 0];

W = rref(W);
pretty(W)