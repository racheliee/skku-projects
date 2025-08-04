sigma = 1;
n = 1000; 

% generate uniform~(0,1) random variables
U = rand(n, 1);

% transform to rayleigh using inverse transform 
V = sqrt(-2 * sigma^2 * log(1 - U));

% plot histogram
binWidth = 0.2;
edges = 0:binWidth:8;
histogram(V, edges, 'Normalization', 'pdf', 'FaceColor', [0.4 0.6 0.8])
hold on

% plot theoretical rayleigh
v = linspace(0, 8, 1000);
pdf_rayleigh = (v ./ sigma^2) .* exp(-v.^2 / (2 * sigma^2));
plot(v, pdf_rayleigh, 'r', 'LineWidth', 2)

% labels
title('Empirical vs Theoretical Rayleigh PDF')
xlabel('v')
ylabel('Probability Density')
legend('Empirical (Histogram)', 'Theoretical PDF')
grid on
