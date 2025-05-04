function a2_1()
    clc;
    fprintf('=== Probability Calculator for Events A and B ===\n');
    
    % Get number of total outcomes
    N = input('Enter total number of outcomes: ');

    % Get probability for each outcome
    fprintf('Enter the probabilities for each of the %d outcomes (as a row vector):\n', N);
    probabilities = input('Example: [0.1 0.2 0.05 ...]: ');

    % Input indices for event A
    A_indices = input('Enter indices for event A (e.g., [1 2 3]): ');
    A_indices = unique(A_indices);

    % Input indices for event B
    B_indices = input('Enter indices for event B (e.g., [3 4 5]): ');
    B_indices = unique(B_indices);

    % Compute event sets
    A = A_indices(:);
    B = B_indices(:);
    
    % Intersection and union
    A_and_B = intersect(A, B);
    A_or_B = union(A, B);
    
    % Compute probabilities
    P_A = sum(probabilities(A));
    P_B = sum(probabilities(B));
    P_AB = sum(probabilities(A_and_B));
    P_AorB = sum(probabilities(A_or_B));
    
    % Conditional probabilities (protect against divide-by-zero)
    P_A_given_B = P_AB / P_B;
    P_B_given_A = P_AB / P_A;

    % Display results
    fprintf('\n--- Results ---\n');
    fprintf('P(A)        = %.4f\n', P_A);
    fprintf('P(B)        = %.4f\n', P_B);
    fprintf('P(A ∩ B)    = %.4f\n', P_AB);
    fprintf('P(A ∪ B)    = %.4f\n', P_AorB);
    fprintf('P(A | B)    = %.4f\n', P_A_given_B);
    fprintf('P(B | A)    = %.4f\n', P_B_given_A);
end
