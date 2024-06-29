# Dynamic Minimum Spanning Tree
알고리즘개론 Project Assignment 3

## Grade
full score (16/10) - additional credit received  
Runtime of the algorithm is less than 1 second for the given input file.

## 1. Objective
Design and implement an algorithm to dynamically maintain the minimum spanning tree (MST) of an initially empty undirected graph, as edges are inserted, deleted, or their weights are changed over time.

## 2. Assumptions and Requirements
1. The graph is initially empty and undirected. Nodes are numbered from 1 to N, where N is the total number of nodes in the graph, specified in the first line of the input file ‘mst.in.’ N is less than or equal to 500.
2. The total number of insertEdge, findMST, deleteEdge, and changeWeight oper- ations is less than or equal to 10,000.
3. Edge weights are positive integers.
4. If the graph is connected, findMST should print the total weight sum of the MST to the output file ‘mst.out.’ If the graph is disconnected, it should print ‘Disconnected.’
5. If an operation is not valid (e.g., inserting an existing edge, deleting a non-existent edge), it should be ignored.

## 3. Example Input & Output
```
Example Input (mst.in):
5
findMST
insertEdge 1 2 4
insertEdge 1 3 3
insertEdge 2 3 2
insertEdge 3 4 1
insertEdge 4 5 5
findMST
deleteEdge 2 3
changeWeight 1 3 6
findMST
insertEdge 2 3 4
findMST
changeWeight 4 5 1
findMST


Expected Output (mst.out):
Disconnected
11
16
14
10
```
## 4. Grading Criteria
1. Correctness and efficiency of your algorithm in maintaining the MST.
2. The total execution time taken by the experiments.
3. Clarity of your documentation, including approach explanation and time complexity analysis.