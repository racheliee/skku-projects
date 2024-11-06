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

## Batching Communication

# DOALL Loop Parallel Schedules

## Static Scheduling

## Global Worklist Work-Stealing Schedule

## Local Worklist Work-Stealing Schedule

## Task Granularity