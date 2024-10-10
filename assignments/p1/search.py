from pacai.util.queue import Queue
from pacai.util.stack import Stack
from pacai.util.priorityQueue import PriorityQueueWithFunction

"""
In this file, you will implement generic search algorithms which are called by Pacman agents.
"""

'''
return 값들:
    Start: (35, 1)
    Is the start a goal?: False
    Start's successors: [((35, 2), 'North', 1), ((34, 1), 'West', 1)]
'''


def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first [p 85].

    Your search algorithm needs to return a list of actions that reaches the goal.
    Make sure to implement a graph search algorithm [Fig. 3.7].

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:
    ```
    print("Start: %s" % (str(problem.startingState())))
    print("Is the start a goal?: %s" % (problem.isGoal(problem.startingState())))
    print("Start's successors: %s" % (problem.successorStates(problem.startingState())))
    ```
    """

    # *** Your Code Here ***
    s = Stack()  # frontier
    visited = set()  # visited states

    start = problem.startingState()

    s.push([start, []])  # [state, path]

    while not s.isEmpty():
        curr_state, curr_path = s.pop()

        if problem.isGoal(curr_state):
            return curr_path

        if curr_state in visited:
            continue

        visited.add(curr_state)

        for new_state, action, cost in problem.successorStates(curr_state):
            s.push([new_state, curr_path + [action]])

    return []


def breadthFirstSearch(problem):
    """
    Search the shallowest nodes in the search tree first. [p 81]
    """

    # *** Your Code Here ***
    q = Queue()  # frontier
    visited = set()  # visited states

    start = problem.startingState()

    q.push([start, []])  # [state, path]

    # print("Start's successors: %s" % (problem.successorStates(problem.startingState())))

    while not q.isEmpty():
        curr_state, curr_path = q.pop()

        if curr_state in visited:
            continue

        visited.add(curr_state)

        for new_state, action, cost in problem.successorStates(curr_state):
            if problem.isGoal(new_state):  # goal check here to expand less nodes
                return curr_path + [action]

            q.push([new_state, curr_path + [action]])

    return []


def uniformCostSearch(problem):
    """
    Search the node of least total cost first.
    """

    def priorityFunction(curr):
        # sort based on cost, tie-breaker is length of path
        return (curr[2], len(curr[1]))

    # *** Your Code Here ***
    # frontier; priority function given with price
    pq = PriorityQueueWithFunction(priorityFunction)
    visited = set()

    start = problem.startingState()

    pq.push([start, [], 0])  # [state, path, cost]

    while not pq.isEmpty():
        curr_state, curr_path, curr_cost = pq.pop()

        if problem.isGoal(curr_state):
            return curr_path

        if curr_state in visited:
            continue

        visited.add(curr_state)

        for new_state, action, cost in problem.successorStates(curr_state):
            if new_state not in visited:
                pq.push([new_state, curr_path + [action], curr_cost + cost])

    return []


def aStarSearch(problem, heuristic):
    """
    Search the node that has the lowest combined cost and heuristic first.
    """

    # *** Your Code Here ***
    # heuristic to use cost + manhattan distance(current state, goal state)
    # tie-breaker is depth (g or length of path)
    pq = PriorityQueueWithFunction(lambda curr: (
        curr[3] + heuristic(curr[0], problem), curr[2]))
    visited = set()

    start = problem.startingState()

    pq.push([start, [], 0, 0])  # [state, path, depth(g), cost(h)]

    while not pq.isEmpty():
        curr_state, curr_path, curr_depth, curr_cost = pq.pop()
        
        # frontier에서 뺄때 goal check!!
        if problem.isGoal(curr_state):
            return curr_path
        
        if curr_state in visited:
            continue

        visited.add(curr_state)

        for new_state, action, cost in problem.successorStates(curr_state):
            # if problem.isGoal(new_state):
            #     return curr_path + [action]

            if new_state not in visited:
                pq.push([new_state, curr_path + [action],
                        curr_depth + 1, curr_cost + cost])

    return []
