N = 10000;

U = rand(N, 1);
V = rand(N, 1);

R = sqrt(-2 * log(U));
X = R .* cos(2 * pi * V);
Y = R .* sin(2 * pi * V);

x_vals = linspace(-4, 4, 100);
std_normal_pdf = (1 / sqrt(2 * pi)) * exp(-x_vals.^2 / 2);

figure;
subplot(1,2,1);
histogram(X, 50, 'Normalization', 'pdf');
hold on;
plot(x_vals, std_normal_pdf, 'r--', 'LineWidth', 1.5);
title('Histogram of X');
xlabel('X'); ylabel('Density');
legend('Sampled', 'N(0,1)');

subplot(1,2,2);
histogram(Y, 50, 'Normalization', 'pdf');
hold on;
plot(x_vals, std_normal_pdf, 'r--', 'LineWidth', 1.5);
title('Histogram of Y');
xlabel('Y'); ylabel('Density');
legend('Sampled', 'N(0,1)');

figure;
scatter(X, Y, 10, 'filled', 'MarkerFaceAlpha', 0.2);
axis equal;
xlabel('X'); ylabel('Y');
title('Scatter Plot of (X, Y)');
grid on;
