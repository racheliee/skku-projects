import random

from pacai.agents.base import BaseAgent
from pacai.agents.search.multiagent import MultiAgentSearchAgent
from pacai.core.directions import Directions


class ReflexAgent(BaseAgent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.
    You are welcome to change it in any way you see fit,
    so long as you don't touch the method headers.
    """

    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        `ReflexAgent.getAction` chooses among the best options according to the evaluation function.

        Just like in the previous project, this method takes a
        `pacai.core.gamestate.AbstractGameState` and returns some value from
        `pacai.core.directions.Directions`.
        """

        # Collect legal moves.
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions.
        scores = [self.evaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best.
        chosenIndex = random.choice(bestIndices)

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current `pacai.bin.pacman.PacmanGameState`
        and an action, and returns a number, where higher numbers are better.
        Make sure to understand the range of different values before you combine them
        in your evaluation function.
        """

        successorGameState = currentGameState.generatePacmanSuccessor(action)

        # Useful information you can extract.
        newPosition = successorGameState.getPacmanPosition()
        # oldFood = currentGameState.getFood()
        newFoodList = successorGameState.getFood().asList()
        # newGhostStates = successorGameState.getGhostStates()
        # newScaredTimes = [ghostState.getScaredTimer() for ghostState in newGhostStates]

        # *** Your Code Here ***

        def manhattanDistance(pos1, pos2):
            return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])

        # note: sometimes gets stuck until the ghost gets close
        # calc distance with food
        foodDist = [manhattanDistance(newPosition, food)
                    for food in newFoodList]
        foodScore = min(foodDist) if foodDist else -1

        # calc distance with ghost and check if ghost is near
        ghostDist = 1
        ghostNearBy = 0
        for g in successorGameState.getGhostPositions():
            temp = manhattanDistance(newPosition, g)
            ghostDist += temp
            if temp < 2:
                ghostNearBy += 1

        # do reciprocal of the values bc we want to maximize the score
        return successorGameState.getScore() + 1.0 / foodScore - 1.0 / ghostDist - ghostNearBy
        # return successorGameState.getScore()


class MinimaxAgent(MultiAgentSearchAgent):
    """
    A minimax agent.

    Here are some method calls that might be useful when implementing minimax.

    `pacai.core.gamestate.AbstractGameState.getNumAgents()`:
    Get the total number of agents in the game

    `pacai.core.gamestate.AbstractGameState.getLegalActions`:
    Returns a list of legal actions for an agent.
    Pacman is always at index 0, and ghosts are >= 1.

    `pacai.core.gamestate.AbstractGameState.generateSuccessor`:
    Get the successor game state after an agent takes an action.

    `pacai.core.directions.Directions.STOP`:
    The stop direction, which is always legal, but you may not want to include in your search.

    Method to Implement:

    `pacai.agents.base.BaseAgent.getAction`:
    Returns the minimax action from the current gameState using
    `pacai.agents.search.multiagent.MultiAgentSearchAgent.getTreeDepth`
    and `pacai.agents.search.multiagent.MultiAgentSearchAgent.getEvaluationFunction`.
    """

    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)

    def getAction(self, gameState):

        def minimax(gameState, depth, agentIndex):
            if depth == self.getTreeDepth() or gameState.isWin() or gameState.isLose():
                return self.getEvaluationFunction()(gameState)
            # maximzing pacman
            if agentIndex == 0:
                return max(
                    minimax(gameState.generateSuccessor(
                        agentIndex, action), depth, agentIndex + 1)
                    for action in gameState.getLegalActions(agentIndex)
                )
            # minimizing ghost
            else:
                nextAgent = agentIndex + 1
                # check if all agents have played
                if gameState.getNumAgents() == nextAgent:
                    nextAgent = 0
                    depth += 1
                return min(
                    minimax(gameState.generateSuccessor(
                        agentIndex, action), depth, nextAgent)
                    for action in gameState.getLegalActions(agentIndex)
                )

        # get actions for pacman
        maxUtil = float('-inf')
        bestAction = None  # best action
        legalActions = [a for a in gameState.getLegalActions(
            0) if a != Directions.STOP]
        for action in legalActions:
            util = minimax(gameState.generateSuccessor(0, action), 0, 1)
            if util > maxUtil:
                maxUtil = util
                bestAction = action

        return bestAction


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    A minimax agent with alpha-beta pruning.

    Method to Implement:

    `pacai.agents.base.BaseAgent.getAction`:
    Returns the minimax action from the current gameState using
    `pacai.agents.search.multiagent.MultiAgentSearchAgent.getTreeDepth`
    and `pacai.agents.search.multiagent.MultiAgentSearchAgent.getEvaluationFunction`.
    """

    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)

    def getAction(self, gameState):

        def minimax(gameState, depth, agentIndex, alpha, beta):
            if depth == self.getTreeDepth() or gameState.isWin() or gameState.isLose():
                return self.getEvaluationFunction()(gameState)

            # maximizing pacman
            if agentIndex == 0:
                v = float('-inf')
                legalActions = [a for a in gameState.getLegalActions(
                    agentIndex) if a != Directions.STOP]
                for a in legalActions:
                    curr = minimax(gameState.generateSuccessor(
                        agentIndex, a), depth, agentIndex + 1, alpha, beta)
                    v = max(v, curr)
                    alpha = max(alpha, curr)
                    if beta <= alpha:
                        break
                return v

            # minimizing ghost
            else:
                v = float('inf')
                nextAgent = agentIndex + 1
                if gameState.getNumAgents() == nextAgent:
                    nextAgent = 0
                    depth += 1

                legalActions = [a for a in gameState.getLegalActions(
                    agentIndex) if a != Directions.STOP]
                for a in legalActions:
                    curr = minimax(gameState.generateSuccessor(
                        agentIndex, a), depth, nextAgent, alpha, beta)
                    v = min(v, curr)
                    beta = min(beta, curr)
                    if beta <= alpha:
                        break
                return v

        bestAction = None
        alpha, beta = float('-inf'), float('inf')
        legalActions = [a for a in gameState.getLegalActions(
            0) if a != Directions.STOP]
        for action in legalActions:
            util = minimax(gameState.generateSuccessor(
                0, action), 0, 1, alpha, beta)
            if util > alpha:
                alpha = util
                bestAction = action

        return bestAction


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
    An expectimax agent.

    All ghosts should be modeled as choosing uniformly at random from their legal moves.

    Method to Implement:

    `pacai.agents.base.BaseAgent.getAction`:
    Returns the expectimax action from the current gameState using
    `pacai.agents.search.multiagent.MultiAgentSearchAgent.getTreeDepth`
    and `pacai.agents.search.multiagent.MultiAgentSearchAgent.getEvaluationFunction`.
    """

    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)

    def getAction(self, gameState):
        def expectimax(gameState, depth, agentIndex):
            if depth == self.getTreeDepth() or gameState.isWin() or gameState.isLose():
                return self.getEvaluationFunction()(gameState)

            # maximizing pacman
            if agentIndex == 0:
                return max(
                    expectimax(gameState.generateSuccessor(
                        agentIndex, action), depth, agentIndex + 1)
                    for action in gameState.getLegalActions(agentIndex)
                )

            # minimizing ghost
            else:
                nextAgent = agentIndex + 1
                if gameState.getNumAgents() == nextAgent:
                    nextAgent = 0
                    depth += 1

                legalActions = [a for a in gameState.getLegalActions(
                    agentIndex) if a != Directions.STOP]
                return sum(
                    expectimax(gameState.generateSuccessor(
                        agentIndex, action), depth, nextAgent)
                    for action in legalActions
                ) / len(legalActions)

        maxUtil = float('-inf')
        bestAction = None
        legalAction = [a for a in gameState.getLegalActions(
            0) if a != Directions.STOP]

        for action in legalAction:
            util = expectimax(gameState.generateSuccessor(0, action), 0, 1)
            if util > maxUtil:
                maxUtil = util
                bestAction = action

        return bestAction


def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable evaluation function.

    DESCRIPTION: <write something here so we know what you did>
    """

    return currentGameState.getScore()


class ContestAgent(MultiAgentSearchAgent):
    """
    Your agent for the mini-contest.

    You can use any method you want and search to any depth you want.
    Just remember that the mini-contest is timed, so you have to trade off speed and computation.

    Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
    just make a beeline straight towards Pacman (or away if they're scared!)

    Method to Implement:

    `pacai.agents.base.BaseAgent.getAction`
    """

    def __init__(self, index, **kwargs):
        super().__init__(index, **kwargs)
