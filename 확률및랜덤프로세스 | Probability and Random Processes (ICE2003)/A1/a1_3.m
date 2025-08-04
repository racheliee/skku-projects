num_samples = 1000; 
nominal_resistance = 1000; 
tolerance = 0.20; 

% Generate 1000 random resistance values within the tolerance
resistance_values = nominal_resistance * (1 + (2 * tolerance * rand(1, num_samples) - tolerance));

% Plot histogram with 10 bins
edges=800:40:1200
histogram(resistance_values, edges);

% Label the plot
xlabel('Resistance (Ω)');
ylabel('Frequency');
title('Histogram of Resistor Values (1000Ω ±20%)');

% Display grid
grid on;