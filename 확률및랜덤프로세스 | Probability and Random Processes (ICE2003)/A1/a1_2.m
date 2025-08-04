num_samples = 5000;
base_thickness = 3;
tolerance = 0.005;

% Generate 5000 random thickness values
thickness_values = base_thickness + (2 * tolerance * rand(1, num_samples) - tolerance);

% Plot histogram with 20 bins
histogram(thickness_values, 20);

% Label the plot
xlabel('Thickness (mm)');
ylabel('Frequency');
title('Histogram of Manufactured Part Thickness');

% Display grid
grid on;
