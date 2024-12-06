---
title: CSE113 Homework 5 Design Document
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
  \fancyhead[L]{CSE 113 Homework 5 Design Document}
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
  This is the design document for the fifth homework assignment of CSE 113: Parallel and Concurrent Programming.
---

\tableofcontents

\newpage

# Part 1: Barrier Throughput

## Overview
This document outlines the design and implementation of the repeated blur operation using three different approaches to barrier synchronization: Spawn and Join, Basic Sense Reversal Barrier (SRBarrier), and Optimized Sense Reversal Barrier (SROBarrier). Each implementation ensures synchronization between threads to perform the repeated blur operation effectively.

The repeated blur operation calculates the average of each array element and its immediate neighbors in parallel. Barriers are used to synchronize threads, ensuring that all threads complete one blur operation before moving to the next.

## Implementation Details

### Spawn and Join (spawn_join.cpp)
- **Purpose**: Launch and join threads for each blur operation. Synchronization is implicit through thread joining.
- **Inputs**:
  - `input`: Pointer to the input array.
  - `output`: Pointer to the output array.
  - `size`: Size of the array.
  - `repeats`: Number of blur repetitions.
  - `num_threads`: Number of threads to spawn.
- **Outputs**:
  - The `output` array contains the blurred values after the final iteration.
- **Assumptions**:
  - `size` is a power of 2.
  - There are no race conditions since threads operate on distinct array chunks.

**Pseudocode**
```plaintext
FUNCTION repeated_blur(input, output, size, repeats, num_threads):
    chunk_size = (size - 2) / num_threads  # Exclude boundaries

    FOR r = 0 TO repeats - 1:
        threads = []

        FOR t = 0 TO num_threads - 1:
            start = 1 + t * chunk_size
            end = (t == num_threads - 1) ? size - 1 : start + chunk_size

            threads.ADD(Thread(blur_chunk, input, output, start, end))

        FOR each thread IN threads:
            thread.JOIN()

        SWAP(input, output)

    IF repeats IS ODD:
        COPY(input TO output)

FUNCTION blur_chunk(input, output, start, end):
    FOR i = start TO end - 1:
        output[i] = (input[i-1] + input[i] + input[i+1]) / 3
```

### Basic Sense Reversal Barrier (SRBarrier.h)
- **Purpose**: Synchronize threads using a sense-reversal barrier to coordinate multiple blur iterations.
- **Inputs**:
  - `input`, `output`, `size`, `repeats`, and `num_threads` as described above.
  - `tid`: Thread ID.
- **Outputs**:
  - Final blurred values in the `output` array.
- **Assumptions**:
  - The barrier correctly synchronizes threads after each iteration.

**Pseudocode**
`sense_reversal_barrier`:
```plaintext
FUNCTION repeated_blur(input, output, size, repeats, num_threads):
    BARRIER_OBJECT.INIT(num_threads)
    chunk_size = (size - 2) / num_threads

    threads = []
    FOR t = 0 TO num_threads - 1:
        start = 1 + t * chunk_size
        end = (t == num_threads - 1) ? size - 1 : start + chunk_size

        threads.ADD(Thread(blur_chunk, input, output, start, end, repeats, t))

    FOR each thread IN threads:
        thread.JOIN()

FUNCTION blur_chunk(input, output, start, end, repeats, tid):
    FOR r = 0 TO repeats - 1:
        FOR i = start TO end - 1:
            output[i] = (input[i-1] + input[i] + input[i+1]) / 3

        BARRIER(tid)

        SWAP(input, output)
```

`SRBarrier`:
```plaintext
CLASS BARRIER_OBJECT:
    FUNCTION INIT(num_threads):
        this.num_threads = num_threads
        counter = 0
        sense = FALSE

    FUNCTION BARRIER(tid):
        old_sense = sense
        pos = counter.FETCH_ADD(1)

        IF pos == num_threads - 1:
            counter = 0
            sense = NOT old_sense
        ELSE:
            WHILE sense == old_sense:
                WAIT()
```

### Optimized Sense Reversal Barrier (SROBarrier.h)
- **Purpose**: Improve the efficiency of the basic sense reversal barrier by reducing contention through relaxed peeking and yielding.
- **Inputs**:
  - Same as the Basic Sense Reversal Barrier.
- **Outputs**:
  - Final blurred values in the `output` array.
- **Assumptions**:
  - Relaxed peeking reduces unnecessary waiting.
  - Threads use `this_thread::yield` to avoid busy-waiting.

**Pseudocode**
```plaintext
CLASS BARRIER_OBJECT:
    FUNCTION INIT(num_threads):
        this.num_threads = num_threads
        counter = 0
        sense = FALSE

    FUNCTION BARRIER(tid):
        old_sense = sense
        pos = counter.FETCH_ADD(1)

        IF pos == num_threads - 1:
            counter = 0
            sense = NOT old_sense
        ELSE:
            WHILE sense == old_sense:
                YIELD()  # Let other threads execute
```

\newpage

# Part 2: Store Buffering on x86 Processors
## Overview
This document details the design and implementation for Part 2 of the assignment, which explores the **Store Buffering Litmus Test** on x86 processors. The goal is to analyze how different synchronization mechanisms (or their absence) impact the observation of weak behaviors. This includes implementing three variations of the test:

1. **Relaxed Store Buffering** (no barriers, relaxed memory order).
2. **Synchronized Store Buffering** (barriers, relaxed memory order).
3. **Disallowed Store Buffering** (barriers, sequential consistency).


## Implementation Details
### Relaxed Store Buffering (relaxed_sb.cpp)
- **Purpose**: Observe weak behaviors (`r0 == 0 && r1 == 0`) allowed by TSO when atomic operations use `memory_order_relaxed` and there is no explicit synchronization between threads.
- **Inputs**:
  - `x`, `y`: Shared atomic variables initialized to `0`.
  - `r0`, `r1`: Registers for storing the loaded values.
  - `B`: Barrier object (not used in this implementation).
- **Outputs**:
  - A histogram of outcomes: counts of `(r0, r1)` for various observed behaviors.
- **Assumptions**:
  - Weak behaviors occur because operations from both threads are not synchronized, exposing relaxed memory semantics.

**Pseudocode**:
```plaintext
FUNCTION t0_function(x, y, r0, B):
    x.store(1, RELAXED)    // Relaxed store to x
    r0 = y.load(RELAXED)   // Relaxed load from y

FUNCTION t1_function(x, y, r1, B):
    y.store(1, RELAXED)    // Relaxed store to y
    r1 = x.load(RELAXED)   // Relaxed load from x
```

### Synchronized Store Buffering (sync_sb.cpp)
- **Purpose**: Increase the likelihood of observing weak behaviors by using barriers to synchronize threads before and after the atomic operations. This ensures that both threads execute their operations concurrently in the critical section.
- **Inputs and Outputs**:
  - Same as Relaxed Store Buffering.
- **Assumptions**:
  - Barriers align the execution of threads, making it more likely to observe the weak behavior allowed by TSO.

**Pseudocode**:
```plaintext
FUNCTION t0_function(x, y, r0, B):
    BARRIER(0)              // Synchronize threads
    x.store(1, RELAXED)      // Relaxed store to x
    r0 = y.load(RELAXED)     // Relaxed load from y
    BARRIER(0)              // Synchronize threads

FUNCTION t1_function(x, y, r1, B):
    BARRIER(1)              // Synchronize threads
    y.store(1, RELAXED)      // Relaxed store to y
    r1 = x.load(RELAXED)     // Relaxed load from x
    BARRIER(1)              // Synchronize threads
```

### Disallowed Store Buffering (sc_sb.cpp)
- **Purpose**: Prevent weak behaviors by enforcing sequential consistency (`memory_order_seq_cst`) and using barriers to synchronize threads. This ensures that all operations appear to be executed in a global sequential order.
- **Inputs and Outputs**:
  - Same as Relaxed Store Buffering.
- **Assumptions**:
  - Sequential consistency prevents weak behaviors, resulting in only strong outcomes (`r0 != 0 || r1 != 0`).

**Pseudocode**:
```plaintext
FUNCTION t0_function(x, y, r0, B):
    BARRIER(0)              // Synchronize threads
    x.store(1, SEQ_CST)      // Sequentially consistent store to x
    r0 = y.load(SEQ_CST)     // Sequentially consistent load from y
    BARRIER(0)              // Synchronize threads

FUNCTION t1_function(x, y, r1, B):
    BARRIER(1)              // Synchronize threads
    y.store(1, SEQ_CST)      // Sequentially consistent store to y
    r1 = x.load(SEQ_CST)     // Sequentially consistent load from x
    BARRIER(1)              // Synchronize threads
```

### Driver Workflow
The `test_driver` function orchestrates the execution of the litmus test and records the observed behaviors. The histogram tracks four possible outcomes:
1. **`r0 == 1 && r1 == 1`**: Both stores and loads occurred sequentially (strong behavior).
2. **`r0 == 1 && r1 == 0`**: Thread 0 completed its store and load before Thread 1’s load.
3. **`r0 == 0 && r1 == 1`**: Thread 1 completed its store and load before Thread 0’s load.
4. **`r0 == 0 && r1 == 0`**: Weak behavior allowed by TSO.

**Pseudocode**:
```plaintext
FUNCTION test_driver(iterations):
    INIT barrier for 2 threads
    histogram = [0, 0, 0, 0]

    FOR i = 0 TO iterations:
        RESET x, y to 0
        LAUNCH threads t0_function and t1_function
        JOIN threads
        RECORD histogram based on (r0, r1)

    RETURN histogram
```

\newpage

# Part 3: Relaxed Behaviours in a Mutex
## Overview
This document outlines the design and implementation of **Dekker’s Algorithm** for mutual exclusion using three variations:
1. **Sequentially Consistent Dekker’s Mutex (SCDekkers)**: Ensures mutual exclusion using `memory_order_seq_cst`.
2. **Relaxed Dekker’s Mutex (RDekkers)**: Uses `memory_order_relaxed` for atomic operations, allowing reordering and weak behaviors.
3. **TSO-Fixed Dekker’s Mutex (TSODekkers)**: Corrects the relaxed mutex under TSO using memory fences (`FENCE`) to enforce order where necessary.

Each implementation must guarantee mutual exclusion while adhering to the respective memory ordering constraints.

## Implementation Details
### Sequentially Consistent Dekker’s Mutex (SCDekkers.h)
- **Purpose**: Implement Dekker's Algorithm with strict sequential consistency to ensure strong ordering and mutual exclusion.
- **Inputs**:
  - `tid`: Thread ID (`0` or `1`).
- **Outputs**:
  - Mutual exclusion between threads.
- **Assumptions**:
  - All atomic operations use `memory_order_seq_cst`, ensuring that operations appear in a global sequential order.

**Pseudocode**:
```plaintext
FUNCTION lock(tid):
    other = 1 - tid
    flag[tid] = TRUE (SEQ_CST)

    WHILE flag[other] IS TRUE:
        IF turn != tid:
            flag[tid] = FALSE (SEQ_CST)
            WAIT UNTIL turn == tid
            flag[tid] = TRUE (SEQ_CST)

FUNCTION unlock(tid):
    flag[tid] = FALSE (SEQ_CST)
    turn = 1 - tid (SEQ_CST)
```

**Design Details**:

- The `lock` function ensures that a thread sets its `flag` before checking the other thread’s `flag`.
- The `unlock` function clears the thread’s `flag` and sets the `turn` to allow the other thread to proceed.
- Sequential consistency guarantees correct ordering of stores and loads.


### Relaxed Dekker’s Mutex (RDekkers.h)
- **Purpose**: Implement Dekker's Algorithm with relaxed memory order (`memory_order_relaxed`) to analyze weak behaviors.
- **Inputs and Outputs**:
  - Same as SCDekkers.
- **Assumptions**:
  - Relaxed operations allow reordering, which may lead to mutual exclusion violations.

**Pseudocode**:
```plaintext
FUNCTION lock(tid):
    other = 1 - tid
    flag[tid] = TRUE (RELAXED)

    WHILE flag[other] IS TRUE:
        IF turn != tid:
            flag[tid] = FALSE (RELAXED)
            WAIT UNTIL turn == tid
            flag[tid] = TRUE (RELAXED)

FUNCTION unlock(tid):
    flag[tid] = FALSE (RELAXED)
    turn = 1 - tid (RELAXED)
```

**Design Details**:

- Atomic operations use `memory_order_relaxed`, allowing them to be reordered by the compiler or hardware.
- Without synchronization, the `flag` and `turn` variables may not be visible across threads immediately, leading to possible mutual exclusion violations.

### TSO-Fixed Dekker’s Mutex (TSODekkers.h)
- **Purpose**: Correct the relaxed mutex under TSO by placing fences (`FENCE`) to enforce memory ordering where needed.
- **Inputs and Outputs**:
  - Same as SCDekkers.
- **Assumptions**:
  - Fences ensure visibility and ordering of `flag` and `turn` operations to preserve mutual exclusion.

**Pseudocode**:
```plaintext
FUNCTION lock(tid):
    other = 1 - tid
    flag[tid] = TRUE (RELAXED)
    FENCE                      // Ensure flag store is visible to other threads

    WHILE flag[other] IS TRUE:
        IF turn != tid:
            flag[tid] = FALSE (RELAXED)
            FENCE              // Ensure flag store is visible before waiting
            WAIT UNTIL turn == tid
            flag[tid] = TRUE (RELAXED)
            FENCE              // Ensure flag store is visible after waiting

FUNCTION unlock(tid):
    flag[tid] = FALSE (RELAXED)
    FENCE                      // Ensure flag store is visible
    turn = 1 - tid (RELAXED)
    FENCE                      // Ensure turn store is visible
```

**Design Details**:

- **Fence Placement**:
  - After `flag[tid]` is set to ensure it is visible to other threads.
  - Before and after waiting for `turn` to prevent reordering of critical sections.
  - After clearing `flag[tid]` and updating `turn` in `unlock` to ensure consistency.
- Fences prevent TSO from reordering the stores and loads, ensuring mutual exclusion.

