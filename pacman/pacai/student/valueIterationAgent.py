from pacai.agents.learning.value import ValueEstimationAgent


class ValueIterationAgent(ValueEstimationAgent):
    """
    A value iteration agent.

    Make sure to read `pacai.agents.learning` before working on this class.

    A `ValueIterationAgent` takes a `pacai.core.mdp.MarkovDecisionProcess` on initialization,
    and runs value iteration for a given number of iterations using the supplied discount factor.

    Some useful mdp methods you will use:
    `pacai.core.mdp.MarkovDecisionProcess.getStates`,
    `pacai.core.mdp.MarkovDecisionProcess.getPossibleActions`,
    `pacai.core.mdp.MarkovDecisionProcess.getTransitionStatesAndProbs`,
    `pacai.core.mdp.MarkovDecisionProcess.getReward`.

    Additional methods to implement:

    `pacai.agents.learning.value.ValueEstimationAgent.getQValue`:
    The q-value of the state action pair (after the indicated number of value iteration passes).
    Note that value iteration does not necessarily create this quantity,
    and you may have to derive it on the fly.

    `pacai.agents.learning.value.ValueEstimationAgent.getPolicy`:
    The policy is the best action in the given state
    according to the values computed by value iteration.
    You may break ties any way you see fit.
    Note that if there are no legal actions, which is the case at the terminal state,
    you should return None.
    """

    def __init__(self, index, mdp, discountRate=0.9, iters=100, **kwargs):
        super().__init__(index, **kwargs)

        self.mdp = mdp
        self.discountRate = discountRate
        self.iters = iters
        # A dictionary which holds the q-values for each state.
        self.values = {state: 0.0 for state in self.mdp.getStates()}

        # Compute the values here.
        # raise NotImplementedError()

        for iteration in range(self.iters):
            new_vals = self.values.copy()

            for state in self.mdp.getStates():
                if self.mdp.isTerminal(state):
                    new_vals[state] = 0.0
                else:
                    possible_actions = self.mdp.getPossibleActions(state)
                    if possible_actions:
                        new_vals[state] = max(self.getQValue(
                            state, action) for action in possible_actions)

            self.values = new_vals

    def getValue(self, state):
        """
        Return the value of the state (computed in __init__).
        """

        return self.values.get(state, 0.0)

    def getAction(self, state):
        """
        Returns the policy at the state (no exploration).
        """

        return self.values.get(state)

    def getQValue(self, state, action):
        q_value = 0.0

        for next_state, prob in self.mdp.getTransitionStatesAndProbs(state, action):
            reward = self.mdp.getReward(state, action, next_state)
            q_value += prob * (reward + self.discountRate * self.getValue(next_state))

        return q_value

    def getPolicy(self, state):
        if self.mdp.isTerminal(state):
            return None
    
        possible_actions = self.mdp.getPossibleActions(state)
        if not possible_actions:
            return None

        best_action = max(possible_actions,
                          key=lambda action: self.getQValue(state, action))
        return best_action
