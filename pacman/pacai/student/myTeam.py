# from pacai.util import reflection
from pacai.student.multiagents import ReflexAgent
from pacai.student.multiagents import AlphaBetaAgent


def createTeam(firstIndex, secondIndex, isRed,
               first='pacai.agents.capture.dummy.DummyAgent',
               second='pacai.agents.capture.dummy.DummyAgent'):
    """
    This function should return a list of two agents that will form the capture team,
    initialized using firstIndex and secondIndex as their agent indexed.
    isRed is True if the red team is being created,
    and will be False if the blue team is being created.
    """

    # firstAgent = reflection.qualifiedImport(first)
    # secondAgent = reflection.qualifiedImport(second)

    # return [
    #     firstAgent(firstIndex),
    #     secondAgent(secondIndex),
    # ]

    return [
        ReflexAgent(firstIndex),
        AlphaBetaAgent(secondIndex),
    ]
