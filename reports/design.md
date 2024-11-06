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

# DOALL Loop Parallel Schedules

## Static Scheduling

## Global Worklist Work-Stealing Schedule

## Local Worklist Work-Stealing Schedule

## Task Granularity