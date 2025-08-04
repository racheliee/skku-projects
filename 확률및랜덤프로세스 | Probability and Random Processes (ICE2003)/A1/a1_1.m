% Parameters
sigma = 1;
n = 1000;  % You can try 10000 later for better approximation

% Step 1: Generate Uniform(0,1) random variables
U = rand(n, 1);

% Step 2: Transform to Rayleigh using inverse transform sampling
V = sqrt(-2 * sigma^2 * log(1 - U));

% Step 3: Plot histogram (normalized to be a PDF)
binWidth = 0.2;
edges = 0:binWidth:8;  % Adjust max based on sample spread
histogram(V, edges, 'Normalization', 'pdf', 'FaceColor', [0.4 0.6 0.8])
hold on

% Step 4: Plot theoretical Rayleigh PDF
v = linspace(0, 8, 1000);
pdf_rayleigh = (v ./ sigma^2) .* exp(-v.^2 / (2 * sigma^2));
plot(v, pdf_rayleigh, 'r', 'LineWidth', 2)

% Labels
title('Empirical vs Theoretical Rayleigh PDF')
xlabel('v')
ylabel('Probability Density')
legend('Empirical (Histogram)', 'Theoretical PDF')
grid on
