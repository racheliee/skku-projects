import random

from pacai.agents.capture.capture import CaptureAgent
from pacai.core.directions import Directions
from collections import Counter

def createTeam(firstIndex, secondIndex, isRed,
               first='pacai.agents.capture.dummy.DummyAgent',
               second='pacai.agents.capture.dummy.DummyAgent'):
    """
    This function should return a list of two agents that will form the capture team,
    initialized using firstIndex and secondIndex as their agent indexed.
    isRed is True if the red team is being created,
    and will be False if the blue team is being created.
    """

    return [
        OffenseAgent(firstIndex),
        DefenseAgent(secondIndex),
    ]


class OffenseAgent(CaptureAgent):
    """
    A sophisticated offensive agent that actively eats food and leverages scared ghost states.
    """

    def registerInitialState(self, gameState):
        """
        Initialize agent-specific variables.
        """
        super().registerInitialState(gameState)
        self.start = gameState.getAgentPosition(self.index)

    def chooseAction(self, gameState):
        """
        Choose the best action based on an evaluation function.
        """
        actions = gameState.getLegalActions(self.index)
        actions.remove(Directions.STOP)  # Remove STOP to encourage movement

        values = []
        for action in actions:
            successor = gameState.generateSuccessor(self.index, action)
            value = self.evaluate(gameState, successor, action)
            values.append((value, action))

        # Choose the action with the highest value
        bestValue = max(values)[0]
        bestActions = [action for value, action in values if value == bestValue]

        return random.choice(bestActions)

    def evaluate(self, currentGameState, successorGameState, action):
        """
        Evaluate the game state to determine the desirability.
        """
        features = self.getFeatures(currentGameState, successorGameState)
        weights = self.getWeights()

        return sum(features[feature] * weights.get(feature, 0) for feature in features)

    def getFeatures(self, currentGameState, successorGameState):
        """
        Compute features for the state.
        """
        features = Counter()
        myState = successorGameState.getAgentState(self.index)
        myPos = myState.getPosition()

        # Food-related features
        foodList = self.getFood(successorGameState).asList()
        capsules = self.getCapsules(successorGameState)
        features['foodEaten'] = len(self.getFood(currentGameState).asList()) - len(foodList)
        features['minFoodDistance'] = min([self.getMazeDistance(myPos, food) for food in foodList])

        if len(capsules) != 0:
            features['minCapsuleDistance'] = min([self.getMazeDistance(myPos, capsule) for capsule in capsules])

        # Ghost-related features
        enemies = [successorGameState.getAgentState(i) for i in self.getOpponents(successorGameState)]
        ghosts = [a for a in enemies if not a.isPacman() and a.getPosition() is not None]
        scaredGhosts = [g for g in ghosts if g.getScaredTimer() > 0]
        activeGhosts = [g for g in ghosts if g.getScaredTimer() == 0]

        # Distance to active ghosts
        if len(activeGhosts) > 0:
            ghostDistances = [self.getMazeDistance(myPos, g.getPosition()) for g in activeGhosts]
            features['distanceToActiveGhost'] = min(ghostDistances)
        else:
            features['distanceToActiveGhost'] = 9999  # No active ghosts nearby

        # Distance to scared ghosts
        if len(scaredGhosts) > 0:
            scaredGhostDistances = [self.getMazeDistance(myPos, g.getPosition()) for g in scaredGhosts]
            features['distanceToScaredGhost'] = min(scaredGhostDistances)
        else:
            features['distanceToScaredGhost'] = 9999

        # Incentivize eating scared ghosts
        if len(scaredGhosts) > 0:
            for g in scaredGhosts:
                if self.getMazeDistance(myPos, g.getPosition()) <= 1:
                    features['scaredGhostEaten'] = 1

        # Check if carrying food and consider returning home
        features['distanceToHome'] = self.getMazeDistance(myPos, self.start)

        return features

    def getWeights(self):
        """
        Assign weights to the features.
        """
        return {
            'foodEaten': 200,  # Strong reward for eating food
            'minFoodDistance': -20,  # Incentivize minimizing total distance to food
            'distanceToActiveGhost': 15,  # Avoid active ghosts
            'distanceToScaredGhost': -5,  # Encourage moving toward scared ghosts
            'scaredGhostEaten': 50,  # Slight reward for eating a scared ghost
            'distanceToHome': -0.5,  # Slight incentive to return home
            'minCapsuleDistance': -30,  # Incentivize capsules over food
        }


class DefenseAgent(CaptureAgent):
    """
    A base class for capture agents.
    This class has some helper methods that students may find useful.

    The recommended way of setting up a capture agent is just to extend this class
    and implement `CaptureAgent.chooseAction`.
    """
    def registerInitialState(self, gameState):
        """
        Initialize agent-specific variables.
        """
        super().registerInitialState(gameState)
        self.start = gameState.getAgentPosition(self.index)
        self.lastObservedFood = None

    def chooseAction(self, gameState):
        """
        Choose the best action based on an evaluation function.
        """
        actions = gameState.getLegalActions(self.index)
        actions.remove(Directions.STOP)  # Remove STOP to keep moving

        values = []
        for action in actions:
            successor = gameState.generateSuccessor(self.index, action)
            value = self.evaluate(successor)
            values.append((value, action))

        # Choose the action with the highest value
        bestValue = max(values)[0]
        bestActions = [action for value, action in values if value == bestValue]

        return random.choice(bestActions)

    def evaluate(self, gameState):
        """
        Evaluate the game state to determine the desirability.
        """
        features = self.getFeatures(gameState)
        weights = self.getWeights(gameState)

        return sum(features[feature] * weights.get(feature, 0) for feature in features)


    def getFeatures(self, gameState):
        """
        Compute features for the state.
        """
        features = Counter()
        myState = gameState.getAgentState(self.index)
        myPos = myState.getPosition()

        # Compute distance to invaders
        enemies = [gameState.getAgentState(i) for i in self.getOpponents(gameState)]
        invaders = [a for a in enemies if a.isPacman() and a.getPosition() is not None]
        features['numInvaders'] = len(invaders)

        if len(invaders) > 0:
            invaderDistances = [self.getMazeDistance(myPos, inv.getPosition()) for inv in invaders]
            minInvaderDist = min(invaderDistances)
            features['invaderDistance'] = float(minInvaderDist) / (gameState.getWalls().getWidth() * gameState.getWalls().getHeight())
        else:
            features['invaderDistance'] = 1.0  # No invaders, focus on patrolling

        # Compute the amount of food being defended
        currentFood = self.getFoodYouAreDefending(gameState).asList()
        if self.lastObservedFood is not None:
            if len(self.lastObservedFood) > len(currentFood):
                features['foodLost'] = 1.0
            else:
                features['foodLost'] = 0.0
        else:
            features['foodLost'] = 0.0

        self.lastObservedFood = currentFood

        # Compute distance to the center of own territory (for patrolling)
        midWidth = gameState.getWalls().getWidth() // 2
        if self.red:
            patrolX = midWidth - 1
        else:
            patrolX = midWidth
        patrolPoints = [(patrolX, y) for y in range(gameState.getWalls().getHeight()) if not gameState.hasWall(patrolX, y)]
        minPatrolDist = min([self.getMazeDistance(myPos, point) for point in patrolPoints])
        features['distanceToPatrol'] = float(minPatrolDist) / (gameState.getWalls().getWidth() * gameState.getWalls().getHeight())

        # # Avoid being in the same position as before
        # if len(self.observationHistory) > 1:
        #     prevGameState = self.observationHistory[-2]
        #     prevPos = prevGameState.getAgentState(self.index).getPosition()
        #     if myPos == prevPos:
        #         features['isStuck'] = 1.0
        #     else:
        #         features['isStuck'] = 0.0
        # else:
        #     features['isStuck'] = 0.0

        return features

    def getWeights(self, gameState):
        """
        Assign weights to the features.
        """
        return {
            'numInvaders': -1000.0,
            'invaderDistance': -100.0,
            'foodLost': -500.0,
            'distanceToPatrol': -10.0,
            'isStuck': -100.0,  # isStuck is currently commented out
        }