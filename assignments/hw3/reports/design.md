---
title: CSE113 Homework 3 Design Document
date: \today
geometry: margin=1in
papersize: letter
fontfamily: newtxtext, newtxmath
fontsize: 10pt
numbersections: true
autoEqnLabels: true
header-includes: |
  \usepackage{authblk}
  \author{Rachel (Younwoo) Park}
  \affil{Dept. of Systems Management Engineering\\Sungkyunkwan University}
  \usepackage{fancyhdr}
  \pagestyle{fancy}
  \fancyhead[L]{CSE 113 Homework 3 Design Document}
  \fancyhead[C]{}
  \fancyhead[R]{\thepage}
  \fancyfoot{}
  \usepackage{float}
  \let\origfigure\figure
  \let\endorigfigure\endfigure
  \renewenvironment{figure}[1][2] {\expandafter\origfigure\expandafter[H]} {
    \endorigfigure
  }
#   \usepackage[hangul, nonfrench, finemath]{kotex}
abstract: |
  This is the design document for the third homework assignment of CSE 113: Parallel and Concurrent Programming.
---

\tableofcontents

\newpage

# Producer-Consumer Queues
## Synchronous Producer-Consumer Queue
The goal is to implement a synchronous producer-consumer queue for a program that processes an array an array of floating-point values by computing the cosine of each value using three communicating threads

### Architecture
**Threads**

- **Memory load thread**: loads values from an input array and stores them in `memory_to_trig`
- **Trig thread**: dequeues values from `memory_to_trig`, computes the cosine of each value, and enqueues the results in `trig_to_memory`
- **Memory store thread**: dequeues values from `trig_to_memory` and writes back to the array

**Queues**

- `memory_to_trig`: queue between memory load thread and trig thread
- `trig_to_memory`: queue between trig thread and memory store thread

### Pseudocode
```cpp
class CQueue {
  private:
    buffer      // Array of floats to store values
    capacity    // Maximum number of elements in the buffer
    front       // Index for dequeueing (consumer)
    back        // Index for enqueueing (producer)
    count       // Atomic variable to track number of elements in the queue

  public:
    // Constructor to initialize the queue
    CQueue(capacity):
      Initialize buffer with size = capacity
      Set front = 0, back = 0, count = 0

    // Enqueue method to add an element
    void enq(float e):
      // Wait until there's space in the queue
      while (count == capacity);
      
      // Add element to the buffer
      buffer[back] = e
      back = (back + 1) % capacity  // Move back pointer to the next position
      
      // Signal that an item is added
      count += 1

    // Dequeue method to remove an element
    float deq():
      // Wait until there's an element in the queue
      while (count == 0); 
      
      // Retrieve element from the buffer
      result = buffer[front]
      front = (front + 1) % capacity  // Move front pointer to the next position
      
      // Signal that an item is removed
      count -= 1
      return result
}
```



## Asynchronous Producer-Consumer Queue
The goal is to implement an asynchronous producer-consumer queue for a program that processes an array of floating-point values by computing the cosine of each value using three communicating threads.

### Architecture
**Threads**

- **Memory Load Thread**: Loads values from an input array and stores them in `memory_to_trig`.
- **Trig Thread**: Dequeues values from `memory_to_trig`, computes the cosine of each value, and enqueues the results in `trig_to_memory`.
- **Memory Store Thread**: Dequeues values from `trig_to_memory` and writes values back to the array.

**Queues**

- **`memory_to_trig`**: Queue between memory load thread and trig thread.
- **`trig_to_memory`**: Queue between trig thread and memory store thread.

### Pseudocode

```cpp
class CQueue {
  private:
    buffer      // Circular array of floats to store values
    front       // Index for dequeueing (consumer)
    back        // Index for enqueueing (producer)
    count       // Atomic variable to track the number of elements in the queue

  public:
    // Constructor to initialize the queue
    CQueue():
      Initialize buffer with size = CQUEUE_SIZE
      Set front = 0, back = 0, count = 0

    // Enqueue method to add a single element asynchronously
    void enq(float e):
      // Wait until there is space in the queue
      while (count == CQUEUE_SIZE);
      
      // Add element to the buffer
      buffer[back] = e
      back = (back + 1) % CQUEUE_SIZE  // Move back pointer to the next position
      
      // Signal that an item is added
      count += 1

    // Dequeue method to remove a single element asynchronously
    float deq():
      // Wait until there is an item in the queue
      while (count == 0); 
      
      // Retrieve element from the buffer
      result = buffer[front]
      front = (front + 1) % CQUEUE_SIZE  // Move front pointer to the next position
      
      // Signal that an item is removed
      count -= 1
      return result
}
```



## Batching Communication

The goal is to implement a batch-based producer-consumer queue that processes an array of floating-point values by computing the cosine of each value using three communicating threads. The program processes data in chunks of 8 elements, reducing communication overhead and improving efficiency.

### Architecture

**Threads**

- **Memory Load Thread**: Loads values from an input array in batches of 8 and stores them in the `memory_to_trig` queue.
- **Trig Thread**: Dequeues batches of 8 values from `memory_to_trig`, computes the cosine of each value, and enqueues the results in the `trig_to_memory` queue.
- **Memory Store Thread**: Dequeues batches of 8 values from `trig_to_memory` and writes them back to the output array.

**Queues**

- **`memory_to_trig`**: The queue connecting the memory load thread and trig thread, used to pass batches of 8 values at a time.
- **`trig_to_memory`**: The queue connecting the trig thread and memory store thread, also used to pass batches of 8 values at a time.

### Pseudocode

```cpp
class CQueue {
  private:
    buffer         // Circular buffer to store values
    capacity       // Maximum number of elements in the buffer
    front          // Index for dequeueing (consumer)
    back           // Index for enqueueing (producer)
    count          // Atomic variable to track the number of elements in the queue

  public:
    // Constructor to initialize the queue
    CQueue(capacity):
      Initialize buffer with size = capacity
      Set front = 0, back = 0, count = 0

    // Batch enqueue method to add 8 elements
    void enq_8(float e[8]):
      // Wait until there's space for 8 elements in the queue
      while (count + 8 > capacity);
      
      // Add each element in the batch to the buffer
      for i in range(0, 8):
        buffer[back] = e[i]
        back = (back + 1) % capacity  // Move back pointer
      
      // Signal that 8 items are added
      count += 8

    // Batch dequeue method to remove 8 elements
    void deq_8(float e[8]):
      // Wait until there are at least 8 elements in the queue
      while (count < 8);
      
      // Retrieve each element in the batch from the buffer
      for i in range(0, 8):
        e[i] = buffer[front]
        front = (front + 1) % capacity  // Move front pointer
      
      // Signal that 8 items are removed
      count -= 8
```

### Thread Workflow
1. **Memory Load Thread**:
   - **Purpose**: To load values from the input array into `memory_to_trig` in batches of 8.
   - **Pseudocode**:
     ```cpp
     void memory_load_thread_func(float *a, int size) {
       for (int i = 0; i < size; i += 8) {
         memory_to_trig.enq_8(a + i);  // Enqueue 8 elements from `a`
       }
     }
     ```
   - **Explanation**: This function iterates through the input array `a` in chunks of 8 elements, calling `enq_8()` to enqueue each batch into `memory_to_trig`.

2. **Trig Thread**:
   - **Purpose**: To retrieve batches of 8 elements from `memory_to_trig`, compute the cosine of each value, and then enqueue the processed values into `trig_to_memory`.
   - **Pseudocode**:
     ```cpp
     void trig_thread_func(int size) {
       float batch[8];
       for (int i = 0; i < size; i += 8) {
         memory_to_trig.deq_8(batch);      // Dequeue 8 elements
         for (int j = 0; j < 8; ++j) {
           batch[j] = cos(batch[j]);       // Compute cosine for each element
         }
         trig_to_memory.enq_8(batch);      // Enqueue the processed batch
       }
     }
     ```
   - **Explanation**: This function dequeues a batch of 8 elements, applies `cos()` to each, and enqueues the processed results into `trig_to_memory`.

3. **Memory Store Thread**:
   - **Purpose**: To dequeue batches of 8 elements from `trig_to_memory` and store them back in the output array `a`.
   - **Pseudocode**:
     ```cpp
     void memory_store_thread_func(float *a, int size) {
       for (int i = 0; i < size; i += 8) {
         trig_to_memory.deq_8(a + i);  // Dequeue 8 elements into `a`
       }
     }
     ```
   - **Explanation**: This function iterates through `a`, dequeuing and storing each batch of 8 values from `trig_to_memory`.


\newpage

# DOALL Loop Parallel Schedules

## Static Scheduling
To parallelize a DOALL loop where each iteration operates independently. The loop’s workload per iteration varies linearly with the loop index, creating a potential load imbalance. Using a static scheduling strategy, we divide the work into chunks, assigning each chunk to a separate thread. The goal is to have each thread perform the same number of iterations, even though the work per iteration differs.

### Architecture
- **Static Partitioning**: The loop range is divided into equal-sized chunks based on the number of threads. Each thread is assigned a contiguous block of iterations, leading to the following partitioning:
  - Thread 0: Iterates over index `0` to `size/num_threads - 1`
  - Thread 1: Iterates over index `size/num_threads` to `2*(size/num_threads) - 1`
  - This pattern continues until each thread has an equal range of `size/num_threads` elements.
  
- **Thread Management**: 
  - Threads are created with `std::thread` and are assigned a specific chunk based on their thread ID.
  - Each thread independently processes its assigned range, with the main function (`launch_threads`) responsible for starting and joining each thread.

**Variables**
- `result_parallel`: Array of floats, where each index `i` is updated with the value of `result_parallel[i]` multiplied `mult[i]` times (using repeated addition).
- `mult`: Array of integers indicating the number of times each index in `result_parallel` should be multiplied by itself (simulated via addition).

### Pseudocode
```cpp
function parallel_mult(result, mult, size, tid, num_threads):
    chunk_size = size / num_threads
    start = tid * chunk_size
    end = start + chunk_size
    if tid == num_threads - 1:
        end = size  # Last thread takes any remaining elements

    for i from start to end:
        base = result[i]
        for j from 0 to mult[i] - 1:
            result[i] = result[i] + base

function launch_threads(result_parallel, mult):
    threads = empty list

    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_mult(result_parallel, mult, SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)
```



## Global Worklist Work-Stealing Schedule
To parallelize a loop with varying workloads across iterations using a global worklist strategy. The global worklist approach with an atomic counter dynamically assigns loop iterations to threads as they request work. This reduces load imbalance, as each thread pulls work from a shared counter until all work is completed.

### Architecture

**Parallel Strategy**: Global Worklist with Atomic Counter

- **Global Counter**: An atomic counter is used to keep track of the next unprocessed loop iteration (`counter`). Each thread uses `counter.fetch_add(1)` to atomically increment and retrieve an index, ensuring unique assignments without race conditions.
- **Dynamic Work Assignment**: Each thread processes an index obtained from `counter` and continues pulling new work until `counter` reaches the array's size. This strategy dynamically balances the load by allowing threads that finish quickly to continue pulling work.

**Thread Management**:

- **Thread Creation and Joining**: Threads are created and managed in `launch_threads`, which launches `NUM_THREADS` threads, each executing `parallel_mult` with the shared `counter`. Threads continue fetching work until the counter surpasses the total size.

**Variables**

- `result_parallel`: Array of floats, where each index `i` is updated with the value of `result_parallel[i]` multiplied `mult[i]` times (computed via repeat additions).
- `mult`: Array of integers indicating the number of times each index in `result_parallel` should be multiplied by itself.

### Pseudocode

```cpp
function parallel_mult(result, mult, size, tid, num_threads):
    while true:
        i = counter.fetch_add(1)  // Atomically get the next available index
        if i >= size:
            break  // Exit if there are no more elements to process
        
        base = result[i]
        for j from 0 to mult[i] - 1:
            result[i] = result[i] + base

function launch_threads(result_parallel, mult):
    threads = empty list

    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_mult(result_parallel, mult, SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)
```




## Local Worklist Work-Stealing Schedule
To parallelize a loop with varying workloads across iterations using a local worklist workstealing approach. Each thread has a local queue (`IOQueue`) that initially holds a unique subset of work items. When a thread’s queue is empty, it attempts to steal work from the queues of other threads. This approach provides dynamic load balancing, allowing idle threads to remain productive by helping other threads complete work.

### Architecture

**Parallel Strategy**: Local Worklist with Work Stealing

- **Local Queues**: Each thread has a local queue (`Q[tid]`), where it enqueues and dequeues tasks independently.
- **Work Stealing**: When a thread's queue is empty, it attempts to steal work from other threads' queues in a round-robin manner. This dynamic task allocation balances the workload by allowing threads to pick up additional tasks if they complete their own early.
- **Finished Threads Tracking**: An atomic counter (`finished_threads`) tracks completed threads. When all threads have finished their tasks and cannot steal more work, execution ends.

**Thread Management**:

- **Thread Creation and Joining**: The `launch_threads` function initializes queues and spawns `NUM_THREADS` threads. Each thread runs `parallel_mult`, where it processes its local tasks and performs work stealing if necessary. Threads continue fetching work until they complete all available tasks.

**Variables**:

- `result_parallel`: Array of floats, where each index `i` is updated by performing `mult[i]` repeat additions.
- `mult`: Array of integers specifying the number of times each `result_parallel[i]` should be added to itself.

### Pseudocode

```cpp
function parallel_mult(result, mult, size, tid, num_threads):
    while Q[tid] has tasks:
        task = Q[tid].deq()
        base = result[task]
        for j from 0 to mult[task] - 1:
            result[task] += base

    // Work stealing loop
    while true:
        if all threads are finished:
            break
        for each other thread's queue:
            task = Q[other_thread].deq()
            if task found:
                base = result[task]
                for j from 0 to mult[task] - 1:
                    result[task] += base
                break  // Exit if task stolen
        if no work found:
            mark thread as finished
            break

function launch_threads(result_parallel, mult):
    initialize queues for each thread
    create threads to execute parallel_mult with work-stealing
    join all threads
```



## Local Worklist Work-Stealing with 32-element Dequeue

In this variation, we use the same local worklist and work-stealing strategy but with an optimization: each thread dequeues and processes tasks in chunks of 32 elements. This approach reduces the frequency of dequeue operations, potentially improving performance, especially for high-contention scenarios.

### Architecture

**Parallel Strategy**: Local Worklist with 32-Element Chunked Work Stealing

- **32-Element Dequeue (`deq_32`)**: Each thread dequeues tasks in chunks of 32. If fewer than 32 tasks are available, the function returns `-1`, indicating the need to attempt stealing from other queues.
- **Work Stealing in Chunks**: When a thread’s queue is empty, it tries to steal 32 tasks from other threads. This minimizes overhead by reducing the number of steal attempts, improving load distribution efficiency.
- **Finished Threads Tracking**: The `finished_threads` counter tracks completed threads, ensuring that all threads finish before terminating.

**Thread Management**:

- **Thread Creation and Joining**: `launch_threads` initializes the task queues, then creates and joins threads as in `part3_local.h`. Each thread runs `parallel_mult`, dequeuing and processing 32-element chunks and performing chunked work-stealing if its queue is empty.

**Variables**:

- `result_parallel`: Array of floats where each index `i` is updated by performing `mult[i]` repeat additions.
- `mult`: Array of integers specifying the number of times each `result_parallel[i]` should be added to itself.
- `tasks[32]`: Temporary array for holding 32 tasks dequeued at once.

### Pseudocode

```cpp
function parallel_mult(result, mult, size, tid, num_threads):
    tasks = array of 32 elements

    // Process 32-element chunks from local queue
    while Q[tid].deq_32(tasks) == 0:
        for each task in tasks:
            base = result[task]
            for j from 0 to mult[task] - 1:
                result[task] += base

    // Work stealing loop
    while true:
        if all threads are finished:
            break
        for each other thread's queue:
            if Q[other_thread].deq_32(tasks) == 0:
                for each task in tasks:
                    base = result[task]
                    for j from 0 to mult[task] - 1:
                        result[task] += base
                break  // Exit if 32 tasks stolen
        if no work found:
            mark thread as finished
            break

function launch_threads(result_parallel, mult):
    initialize queues for each thread
    create threads to execute parallel_mult with 32-element work-stealing
    join all threads
```
