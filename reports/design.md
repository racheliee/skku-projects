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

---

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

---

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

---

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

---


## Local Worklist Work-Stealing Schedule
To parallelize a loop with varying workloads across iterations using a local worklist workstealing approach. Each thread has a local queue (`IOQueue`) that initially holds a unique subset of work items. When a thread’s queue is empty, it attempts to steal work from the queues of other threads. This approach provides dynamic load balancing, allowing idle threads to remain productive by helping other threads complete work.

### Architecture
#### local.h
**Parallel Strategy**: Local Worklist with Workstealing
- **Local Queues**: Each thread has its own local queue (`IOQueue`) which initially contains a unique set of work items (indices to process). Threads operate on their local queues by dequeuing work until their queue is empty.
- **Workstealing**: If a thread’s local queue is empty, it attempts to “steal” work from other threads' queues. This dynamic work assignment keeps threads busy even if they finish their initial assigned tasks early.
- **Global Variables**: 
  - `Q`: An array of `IOQueue` instances, one for each thread.
  - `finished_threads`: An atomic counter to track the number of threads that have finished processing.

**Thread Management**:
1. **Queue Initialization**:
   - In `parallel_enq`, each thread enqueues its assigned indices into its local queue in a chunked manner.
   - Each queue is initialized with sufficient space to hold the assigned indices.
2. **Parallel Processing**:
   - The `parallel_mult` function performs the main computation, where threads dequeue tasks from their local queue and process them. If a thread’s queue is empty, it attempts to steal work from other threads.

**Variables**
- `result_parallel`: Array of floats, where each index `i` is updated with the value of `result_parallel[i]` multiplied `mult[i]` times (computed via repeat additions).
- `mult`: Array of integers indicating the number of times each index in `result_parallel` should be multiplied by itself.

#### IOQueue.h
**Concurrent Queue Structure**:
- The queue uses a circular buffer for efficient storage and wraparound of indices.
- Two atomic pointers, `head` and `tail`, keep track of the dequeuing and enqueuing positions, respectively. This allows threads to safely enqueue and dequeue items without overlap.

**Atomic Variables**:
- `head`: Points to the next element to be dequeued.
- `tail`: Points to the next position where a new element can be enqueued.

**Private Variables**:
- `buffer`: The array that stores elements in a circular fashion.
- `capacity`: The maximum number of elements that the queue can hold.

### Functions
#### local.h
1. **`parallel_enq`**: Initializes local queues for each thread with assigned indices.
   - **Inputs**: `size` (total size of the array), `tid` (thread ID), and `num_threads` (total number of threads).
   - **Operation**:
     - Each thread enqueues its own chunk of indices into its local queue `Q[tid]`.
     - Indices are assigned in contiguous chunks similar to static partitioning.

2. **`parallel_mult`**: Performs parallel multiplication with workstealing.
   - **Inputs**: `result_parallel` (array), `mult` (multiplication counts), `size` (total size of the array), `tid` (thread ID), and `num_threads` (total number of threads).
   - **Operation**:
     - Each thread dequeues indices from its local queue and processes them.
     - If the queue is empty, the thread attempts to steal work from other threads’ queues.
     - When a thread finishes, it increments `finished_threads`.
     - Threads continue attempting to work or steal until all threads have completed.

3. **`launch_threads`**: Manages thread creation and joining.
   - **Operation**:
     - First, initializes the queues by calling `parallel_enq` in parallel for each thread.
     - Next, calls `parallel_mult` in parallel for each thread.
     - Joins all threads at the end of each phase.

#### IOQueue.h
4. **`init(int size)`**:
   - Initializes the queue with a specified size by setting the buffer capacity, allocating memory, and initializing `head` and `tail` to `0`.

5. **`enq(int e)`**:
   - Enqueues a single element `e` into the queue.
   - Checks if there is space by comparing `tail` and `head`.
   - If there’s space, the element is added to the `tail` position, and `tail` is incremented atomically.

6. **`deq()`**:
   - Dequeues a single element from the queue.
   - Returns the value at `head` and increments `head`.
   - If the queue is empty (`head` == `tail`), it returns `-1`.

### Pseudocode for local.h

```cpp
function parallel_enq(size, tid, num_threads):
    chunk_size = size / num_threads
    start = tid * chunk_size
    end = start + chunk_size
    if tid == num_threads - 1:
        end = size  // Last thread takes any remaining elements

    for i from start to end:
        Q[tid].enq(i)  // Enqueue index i to the local queue for this thread

function parallel_mult(result, mult, size, tid, num_threads):
    while true:
        index = Q[tid].deq()  // Dequeue from this thread's local queue
        if index == -1:  // Queue is empty, attempt to steal work
            success = false
            for other_tid from 0 to num_threads - 1:
                if other_tid != tid:
                    index = Q[other_tid].deq()  // Attempt to steal work
                    if index != -1:
                        success = true
                        break  // Exit loop if work is found
            if not success:  // No work available in any queue
                if finished_threads.fetch_add(1) == num_threads - 1:
                    return  // All threads are done
                finished_threads.fetch_sub(1)
                continue  // Retry
        else:
            base = result[index]
            for j from 0 to mult[index] - 1:
                result[index] += base

function launch_threads(result_parallel, mult):
    threads = empty list

    // Initialize queues in parallel
    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_enq(SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)

    threads = empty list  // Reset thread list

    // Launch threads for parallel multiplication with workstealing
    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_mult(result_parallel, mult, SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)
```

---


## Task Granularity
To parallelize a loop with varying workloads across iterations using a local worklist workstealing approach with 32-element batching. Each thread has a local queue (`IOQueue`) initially holding a subset of work items (indices). Threads dequeue work in batches of 32 to reduce the number of queue operations and improve throughput. If a thread’s queue is empty, it attempts to steal batches of 32 elements from other threads' queues.

### Architecture

**Parallel Strategy**: Local Worklist with Workstealing and Batch Processing
- **Local Queues**: Each thread has a local queue (`IOQueue`) initialized with unique indices to process.
- **Workstealing with Batching**: Each thread dequeues and processes 32 elements at a time. If a thread’s queue is empty, it tries to steal 32-element batches from other threads’ queues. This approach ensures efficient processing and minimizes queue operation overhead.
- **Global Variables**: 
  - `Q`: Array of `IOQueue` instances (one for each thread) for local worklists.
  - `finished_threads`: Atomic counter to track the number of threads that have finished processing.

**Thread Management**:
1. **Queue Initialization**:
   - The `parallel_enq` function initializes each thread’s queue by enqueuing a unique range of indices in chunks.
2. **Parallel Processing**:
   - The `parallel_mult` function performs repeated addition, where threads dequeue and process batches of 32 elements. If a thread’s queue is empty, it attempts to steal 32-element batches from other threads.

**Variables**
- `result_parallel`: Array of floats where each index `i` is updated with the value of `result_parallel[i]` multiplied `mult[i]` times (using repeat additions).
- `mult`: Array of integers indicating the number of times each index in `result_parallel` should be multiplied.

`IOQueue.h` has the same structure as in the previous local worklist workstealing approach, with the addition of a `deq_32` function that dequeues 32 elements at a time.

### Function Breakdown
#### local.h
1. **`parallel_enq`**: Initializes local queues for each thread with assigned indices.
   - **Inputs**: `size` (total size of the array), `tid` (thread ID), `num_threads` (total number of threads).
   - **Operation**:
     - Each thread enqueues its chunk of indices into its local queue, `Q[tid]`.
     - Indices are assigned in contiguous chunks similar to static partitioning.

2. **`parallel_mult`**: Processes work in 32-element batches with workstealing.
   - **Inputs**: `result_parallel` (array), `mult` (multiplication counts), `size` (total size of the array), `tid` (thread ID), `num_threads` (total number of threads).
   - **Operation**:
     - Each thread dequeues a batch of 32 elements from its local queue and processes them.
     - If the local queue is empty, the thread attempts to steal a batch of 32 elements from other threads.
     - When a thread completes its work, it increments `finished_threads`. Threads continue working or stealing until all threads complete.

3. **`launch_threads`**: Manages thread creation and joining.
   - **Operation**:
     - First, initializes the queues by calling `parallel_enq` in parallel.
     - Next, calls `parallel_mult` in parallel.
     - Joins all threads at the end of each phase.

#### IOQueue.h
4. **`deq_32(int ret[32])`**:
   - Attempts to dequeue 32 elements in one operation.
   - If there are fewer than 32 elements in the queue, it returns `1` (error code).
   - If 32 elements are available, it copies them to `ret` and updates `head`.

### Pseudocode

```cpp
function parallel_enq(size, tid, num_threads):
    chunk_size = size / num_threads
    start = tid * chunk_size
    end = start + chunk_size
    if tid == num_threads - 1:
        end = size  // Last thread takes any remaining elements

    for i from start to end:
        Q[tid].enq(i)  // Enqueue index i to the local queue for this thread

function parallel_mult(result, mult, size, tid, num_threads):
    batch = empty array of size 32
    while true:
        status = Q[tid].deq_32(batch)  // Try to dequeue 32 elements from local queue
        if status == -1:  // Local queue is empty, attempt to steal work
            success = false
            for other_tid from 0 to num_threads - 1:
                if other_tid != tid:
                    status = Q[other_tid].deq_32(batch)  // Try stealing 32 elements
                    if status != -1:
                        success = true
                        break
            if not success:  // No work available in any queue
                if finished_threads.fetch_add(1) == num_threads - 1:
                    return  // All threads are done
                finished_threads.fetch_sub(1)
                continue  // Retry
        else:
            for each index in batch:
                base = result[index]
                for j from 0 to mult[index] - 1:
                    result[index] += base

function launch_threads(result_parallel, mult):
    threads = empty list

    // Initialize queues in parallel
    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_enq(SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)

    threads = empty list  // Reset thread list

    // Launch threads for parallel multiplication with workstealing
    for tid from 0 to NUM_THREADS - 1:
        create thread to execute parallel_mult(result_parallel, mult, SIZE, tid, NUM_THREADS)
        add thread to threads list

    for each thread in threads:
        wait for thread to complete (join)
```
