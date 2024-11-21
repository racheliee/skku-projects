"""
Analysis question.
Change these default values to obtain the specified policies through value iteration.
If any question is not possible, return just the constant NOT_POSSIBLE:
```
return NOT_POSSIBLE
```
"""

NOT_POSSIBLE = None


def question2():
    """
    [Enter a description of what you did here.]
    Updated parameters to encourage agent to cross the bridge
    """

    answerDiscount = 0.9
    answerNoise = 0.0

    return answerDiscount, answerNoise


def question3a():
    """
    [Enter a description of what you did here.]
    Added a penalty for each step to encourage faster decision making
    """

    answerDiscount = 0.9
    answerNoise = 0.0
    answerLivingReward = -4.0

    return answerDiscount, answerNoise, answerLivingReward


def question3b():
    """
    [Enter a description of what you did here.]
    reduced discount rate to encourage agent to take immediate rewards
    introduced penalty for delays
    """

    answerDiscount = 0.5
    answerNoise = 0.4
    answerLivingReward = -2.0

    return answerDiscount, answerNoise, answerLivingReward


def question3c():
    """
    [Enter a description of what you did here.]
    Left it at longterm focus with 0.9 discount rate
    lowered noise to toerlate risks
    """

    answerDiscount = 0.9
    answerNoise = 0.0
    answerLivingReward = 0.0

    return answerDiscount, answerNoise, answerLivingReward


def question3d():
    """
    [Enter a description of what you did here.]
    added noise to add caution to avoid the cliff
    encouraged faster decision making with a penalty for each step
    """

    answerDiscount = 0.9
    answerNoise = 0.3
    answerLivingReward = -0.1

    return answerDiscount, answerNoise, answerLivingReward


def question3e():
    """
    [Enter a description of what you did here.]
    high noise to avoid exists
    strong penalty for each step to discourage movement
    """

    answerDiscount = 0.9
    answerNoise = 0.9
    answerLivingReward = -1.0

    return answerDiscount, answerNoise, answerLivingReward


def question6():
    """
    [Enter a description of what you did here.]
    There is no combination of epsilon and learning rate that will guarantee the optimal policy
    (with a high probability than 99%)
    """

    # answerEpsilon = None
    # answerLearningRate = None

    # return answerEpsilon, answerLearningRate
    return NOT_POSSIBLE


if __name__ == '__main__':
    questions = [
        question2,
        question3a,
        question3b,
        question3c,
        question3d,
        question3e,
        question6,
    ]

    print('Answers to analysis questions:')
    for question in questions:
        response = question()
        print('    Question %-10s:\t%s' % (question.__name__, str(response)))
