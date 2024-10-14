---
title: CSE113 Homework 1 Design Document
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
  \fancyhead[L]{Markdown to PDF report}
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

  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment.
---

\tableofcontents

\newpage



# 1. Part 1: Loop Unrolling Independent Iterations for ILP

## 1.1 Intuition
The loop structure provided in the assignment is chracterized by a dependency chain of length 64. Since the dependency chain cannot be re-ordered or pre-computed due to the non-associative nature of floating point addition, the loop iterations are dependent on each other. 

While the instructions themselves are not independent, each chain of computation is independent of the other chains. Hence, we can unroll the loop to increase the number of independent instructions that can be executed in parallel.

## 1.2 Approach
Let `SEQ(i)` denote the `i-th` iteration of the sequential loop.

In other words, the sequential loop can be represented as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i++) {
        SEQ(i);
    }
}
```

### 1.2.1 Sequential Loop Unrolling
While the instructions within the sequence are dependent of each other, each sequence themselves does not depend on each other. Therefore, we can execute several sequences in parallel in one iteration of the loop.

#### Pseudocode
For instance, if the loop were to be unrolled by a factor of $k$, the loop would look as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i += k) {
        SEQ(i);
        SEQ(i+1);
        ...
        SEQ(i+k-1);
    }
}
```

### 1.2.2 Interleaved Loop Unrolling
To take advantage of the ILP even further, the instructions of each sequence can be interleaved with one another. This way, the dependent instructions will be spaced further apart, allowing for more instructions to be executed in parallel.

#### Pseudocode
For instance, if the loop were to be unrolled by a factor of $k$, the loop would look as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i += k) {
        float tmp0 = b[i];
        float tmp1 = b[i+1];
        float tmp2 = b[i+2];
        // ...
        float tmpk = b[i+k-1];

        tmp0 += 1.0f; tmp1 += 1.0f; tmp2 += 1.0f; ... tmpk += 1.0f;
        tmp0 += 2.0f; tmp1 += 2.0f; tmp2 += 2.0f; ... tmpk += 2.0f;
        // ...
        tmp0 += N; tmp1 += N; tmp2 += N; ... tmpk += N; // N is the length of the chain
    }
}
```

# 2. Part 2: Unrolling Reduction Loops for ILP
## 2.1 Intuition
Reduction loops are loops where the entire computation is dependent on the previous iteration. By separating the large dependent chain into multiple independent chains of computation, we can create several control flow paths that can be executed in parallel.

## 2.2 Approach to reduction loops
The loop can be unrolled by a factor of $k$ if $k$ accumulators are used to store the intermediate results of the computation. Then, the accumulators can be combined at the end of the loop sequence to obtain the final result. Since the accumulators are independent of each other, they can be computed in parallel.

### 2.3 Pseudocode
Let `REDUCE(a, b)` denote the reduction operation that combines the two values `a` and `b`.
```c
// unrolling
acc0 = a[0]; acc1 = a[SIZE/2]; acc2 = a[SIZE/2 * 2]; ... accSIZE/2 = a[SIZE/2 * (SIZE/2)];
for (int i= 1; i ‹ SIZE/2; i++) {
    acc0 = REDUCE(acc0, a[i]);
    acc1 = REDUCE(acc1, a[i+SIZE/2 * 1]);
    // ...
    accSIZE/2 = REDUCE(accSIZE/2, a[i+SIZE/2 * (SIZE/2)]);
}

// combining the accumulators
final_result = REDUCE(acc0, acc1, acc2, ..., accSIZE/2);
```

# 3. Part 3: SPMD Parallel Programming Using C++ Threads
## 3.1 Intuition
In order to implement Single Program Multiple Data (SPMD) parallel programming, C++ threads can be used to execute the same program on different data. By varying the ways the threads access the data of the array and increment it, it will be possible to distribute the computation across multiple threads.

## 3.2 Approach
### 3.2.1 Sequential Increment
Simply increment each element of the array by $k$ sequentially.

#### Pseudocode
```c
void sequential_increment(volatile int * a, int size, int k) {
    for (each element in a) {
        increment each element k times
    }
}
```

### 3.2.2 Round Robin Increment
In order to distribute the threads to access the array elements in a round-robin fashion, each thread will be assigned to a specific element of the array based on their thread ID (`tid`). The thread will then increment the element until the element is equal to $k$.

#### Pseudocode
- Each thread will be handling `size / num_threads` elements of the array.
- Each thread handles elements at `i % num_threads == tid` where `i` is the index of the element in the array.
```c
void round_robin_increment(volatile int * b, int size, int k, int tid, int num_threads) {

    // declare lambda function to increment the element
    auto increment = [b, size, k, num_threads](int tid) {
        for (int i = tid; i < size; i += num_threads) {
            increment element i k times
        }
    };

    // create threads
    vector<threads> threads;
    for number of threads {
        create/launch threads, assign them to `increment`, and push to vector
    }

    // join threads
    for each thread in threads {
        join the thread
    }
}
```

### 3.2.3 Custom Increment
A more efficient way to distribute the threads is to assign each threads to a specific range of the array, or contiguous blocks of the array. This will improve the cache locality of the threads and reduce the number of cache misses. I presume that this will work better than the round-robin approach because each thread will be more likely to work on the same cache line, reducing the number of cache misses. 

## 3.3 Pseudocode
- The size of the contiguous block of the array that each thread will handle is `size / num_threads`.
```c
void custom_increment(volatile int *c, int size, int k, int num_threads){
    // declare lambda function to increment the element
    auto increment = [c, size, k, num_threads](int tid) {
        for (int i = tid * (size / num_threads); i < (tid + 1) * (size / num_threads); i++) {
            increment element i k times
        }
    };

    // create threads
    vector<threads> threads;
    for number of threads {
        create/launch threads, assign them to `increment`, and push to vector
    }

    // join threads
    for each thread in threads {
        join the thread
    }
}
```