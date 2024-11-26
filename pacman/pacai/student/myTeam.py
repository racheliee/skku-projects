import abc

from pacai.util import util
from pacai.agents.capture.capture import CaptureAgent

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
    A base class for capture agents.
    This class has some helper methods that students may find useful.

    The recommended way of setting up a capture agent is just to extend this class
    and implement `CaptureAgent.chooseAction`.
    """

    def __init__(self, index, timeForComputing = 0.1, **kwargs):
        super().__init__(index, **kwargs)

        # Whether or not you're on the red team
        self.red = None

        # Agent objects controlling you and your teammates
        self.agentsOnTeam = None

        # Maze distance calculator
        self.distancer = None

        # A history of observations
        self.observationHistory = []

        # Time to spend each turn on computing maze distances
        self.timeForComputing = timeForComputing

    @abc.abstractmethod
    def chooseAction(self, gameState):
        """
        Override this method to make a good agent.
        It should return a legal action within the time limit
        (otherwise a random legal action will be chosen for you).
        """

        actions = gameState.getLegalActions(self.index)
        bestAction = None
        maxScore = float('-inf')
        
        for action in actions:
            successor = gameState.generateSuccessor(self.index, action)
            score = self.evaluateOffense(successor)
            if score > maxScore:
                maxScore = score
                bestAction = action
        
        return bestAction

    def evaluateOffense(self, gameState):
        """
        Evaluate the offense strategy.
        """
        pass


class DefenseAgent(CaptureAgent):
    """
    A base class for capture agents.
    This class has some helper methods that students may find useful.

    The recommended way of setting up a capture agent is just to extend this class
    and implement `CaptureAgent.chooseAction`.
    """

    def __init__(self, index, timeForComputing = 0.1, **kwargs):
        super().__init__(index, **kwargs)

        # Whether or not you're on the red team
        self.red = None

        # Agent objects controlling you and your teammates
        self.agentsOnTeam = None

        # Maze distance calculator
        self.distancer = None

        # A history of observations
        self.observationHistory = []

        # Time to spend each turn on computing maze distances
        self.timeForComputing = timeForComputing

    @abc.abstractmethod
    def chooseAction(self, gameState):
        """
        Override this method to make a good agent.
        It should return a legal action within the time limit
        (otherwise a random legal action will be chosen for you).
        """

        actions = gameState.getLegalActions(self.index)
        bestAction = None
        maxScore = float('-inf')
        
        for action in actions:
            successor = gameState.generateSuccessor(self.index, action)
            score = self.evaluateDefense(successor)
            if score > maxScore:
                maxScore = score
                bestAction = action
        
        return bestAction

    def evaluateDefense(self, gameState):
        """
        Evaluate the defense strategy.
        """
        pass
