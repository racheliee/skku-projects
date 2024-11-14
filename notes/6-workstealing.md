# DOALL loops, static scheduling, workstealing

## DOALL loops
- A DOALL loop is a loop where each iteration is independent of the others
- iterations can be done in any order and get the same results
  - hence, they can be done in parallel!!
  - assign each thread a range of iterations to work on

### nested loops
- 