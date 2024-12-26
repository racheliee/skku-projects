# DOALL loops, static scheduling, workstealing

## DOALL loops
- A DOALL loop is a loop where each iteration is independent of the others
- iterations can be done in any order and get the same results
  - hence, they can be done in parallel!!
  - assign each thread a range of iterations to work on

### making outer loops independent
- consider special type of for-loop
  - operates on N dimensional arrays
  - array bases are disjoint and constant
  - bounds, indexes are function of loop variabels, input vars, and constants
  - loops increment by 1

ex: matrix multiplication
```c
for (int i = 0; i < N; i++) 
  for (int j = 0; j < M; j++)
    for (int k = 0; k < P; k++) 
      A[i][j][k] = B[i][j][k] + C[i][j][k];
```

#### safety criteria
- every iteration of the outer-most loop must be independent
  - must produce same result regardless of order of the iterations
- write-write conflicts
  - two distinct iterations write different values to the same location
  - <img src="./assets/6-1.png" width="500">
- read-write conflicts
  - two distinct iterations where one iteration reads from the location written to by another iteration
  - <img src="./assets/6-2.png" width="500">

#### example???
```
for(int i = 0; i < 128; ++i)
  a[i] = a[i] * 2; 

for(int i = 0; i < 128; ++i)
  a[i] = a[0] * 1;
```

## Parallel Schedules
```c
for(int i = 0; i < N; ++i)
  A[i] = B[i] + C[i];
```
- array 인덱스를 round-robin으로 분배 vs. contiguous block으로 분배
  - contiguous block이 더 좋음!!
  - avoid false sharing

### Static scheduling
- works well when loop iterations take similar amounts of time
- say `SIZE / NUM_THREADS` iterations to each thread

```c
for(int x = 0; x < SIZE; ++x){
  //roughly equal time
}

// 아래 방식으로 변경
// needs separate function to adapt the thread spawn and join
void parallel_loop (..., int tid, int num_threads) {
  int chunk_size = (SIZE + num_threads - 1) / num_threads;
  int start = tid * chunk_size;
  int end = min(start + chunk_size, SIZE);

  for (int x = start; x < end; ++x) {
    // do something
  }
}
```


## Unbalanced workloads
- tasks are not balanced
- appears in lots of emerging workloads
- independent iterations have different amounts of work to compute
  - threads w longer tasks take longer to finish
  - threads w shorter tasks finish early --> idle (under-utilized)

```c
for(x = 0; x < SIZE; ++x) {
  for(y = x; y < SIZE; ++y) {
    a[x][y] = b[x][y] + c[x][y];
  }
}
```

- $total\ work =\sum_{n=0}^{SIZE} n$
- work done by the first thread: $t1\ work=\sum_{n=0}^{SIZE/2} n$
- work done by the second thread: $t2\ work=total\ work - t1\ work$
- if size = 64
  - t1 does about 3 times more work than t2
  - only provides 1.3x speedup

### workstealing
- global implicit worklist
- pros: simple implementation
- cons: high contention on global counter, potentially bad memory locality

### global worklists
- threads take tasks dynamically
- if done with a task, take another from the global worklist

```c
// have another function to spawn and join threads
atomic_int x(0);
void parallel_loop (...) {
  for(x = atomic_fetch_add(&x, 1); x < SIZE; atomic_fetch_add(&x, 1)){
    // do dynamic work
  }
}
```

### local worklists
- more difficult to implement
- low contention on local data structures
- potentially better cache locality
- divides tasks into different worklists for each thread
  - if thread finishes its work, it can steal from other threads

```c
concurrent_queue cq[NUM_THREADS]; // global array of concurrent queues (IO queue)
void foo(){
  // initialize queue
  // join threads
  // launch loop function
  // join loop threads
}

atomic_int finished_threads(0);
void parallel_enq(..., int tid, int num_threads){
  int task = 0;
  for(x = cq[tid].deq(); x != -1; x = cq[tid].deq()){
    // do work
  }
  atomic_fetch_add(&finished_threads, 1);
  while(finished_threads.load() != num_threads) {
    int target = // pick a random thread
    int task = cq[target].deq();
    if(task != -1){
      // do work
    }
  }
}
```