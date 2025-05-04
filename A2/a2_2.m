function a2_2(trials)
    if nargin < 1
        trials = 1e6; % default number of simulations
    end
    
    rng('shuffle');

    % (a) At least 1 six in 6 dice rolls
    countA = 0;
    % (b) At least 2 sixes in 12 dice rolls
    countB = 0;
    % (c) At least 3 sixes in 18 dice rolls
    countC = 0;

    for i = 1:trials
        rollsA = randi(6, 1, 6);     % 6 dice
        rollsB = randi(6, 1, 12);    % 12 dice
        rollsC = randi(6, 1, 18);    % 18 dice

        if sum(rollsA == 6) >= 1
            countA = countA + 1;
        end
        if sum(rollsB == 6) >= 2
            countB = countB + 1;
        end
        if sum(rollsC == 6) >= 3
            countC = countC + 1;
        end
    end

    % Estimate probabilities
    pA = countA / trials;
    pB = countB / trials;
    pC = countC / trials;

    fprintf('Estimated probabilities after %d trials:\n', trials);
    fprintf('(a) P(at least 1 six in 6 rolls)     = %.4f\n', pA);
    fprintf('(b) P(at least 2 sixes in 12 rolls)  = %.4f\n', pB);
    fprintf('(c) P(at least 3 sixes in 18 rolls)  = %.4f\n', pC);
end
