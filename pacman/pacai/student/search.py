from pacai.util.queue import Queue

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
    raise NotImplementedError()

def breadthFirstSearch(problem):
    """
    Search the shallowest nodes in the search tree first. [p 81]
    """
    
    # *** Your Code Here ***
    q = Queue() # frontier
    visited = set() # visited states
    
    start = problem.startingState()
    
    q.push([start, []]) # [state, path]
    
    while not q.isEmpty():
        curr_state, curr_path = q.pop()
        
        if problem.isGoal(curr_state):
            return curr_path
        
        if curr_state in visited:
            continue
        
        visited.add(curr_state)
        
        for new_state, action, cost in problem.successorStates(curr_state):
            q.push([new_state, curr_path + [action]])
    
    
    return []

def uniformCostSearch(problem):
    """
    Search the node of least total cost first.
    """

    # *** Your Code Here ***
    raise NotImplementedError()

def aStarSearch(problem, heuristic):
    """
    Search the node that has the lowest combined cost and heuristic first.
    """

    # *** Your Code Here ***
    raise NotImplementedError()
