---
title: CSE113 Homework 2 Design Document
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
  \fancyhead[L]{CSE 113 Homework 2 Design Document}
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
  This is the design document for the second homework assignment of CSE113: Parallel and Concurrent Programming.
---

\tableofcontents

\newpage

# Implementing Mutexes
## Filter Lock
Perterson lock uses two-elemetn boolean flag array to indicate whether a thread is tying to enter the critical setion. To generalize this to $n$ threads (i.e. implement the filter lock), each thread must pass though `n-1` levels of 'exclusion' to enter the critical section. 

### Constructor & Initialization
To implement the filter lock, two arrays -- one for the level and one for the victim -- will be needed. Each thread will start at level 0 and increment the level as it passes through the exclusion levels. The thread will only enter the critical section when all other threads are at a higher level.

**Pseudocode**
```cpp
class mutex {
 public:
  mutex() {} // constructor is not needed
  
  // Initialize the level and victim arrays 
  // Initialized to 0 and resized to the number of threads
  void init (int num_threads) {
    _num_threads = num_threads; 
    level.resize(num_threads, 0);
    victim.resize(num_threads, 0);
  }

 private:
    vector<atomic<int>> level;
    vector<atomic<int>> victim;
    int _num_threads;
};
```

### Lock

![Figure 2.8 (textbook)](./assets/d-1.png){ width=80% }

As shown in the figure above, the lock function will increment the level of the thread to `n-1` and set the victim to itself. The thread will then wait until all other threads are at a higher level and the victim is itself. 

**Pseudocode**

```cpp
void lock(int thread_id) {
    for (int i  = 1; i < num_threads; ++i) {
        // attemtp to enter level i
        level[thread_id] = i;
        victim[i] = thread_id;

        // spin until all threads with a lower level or with the same level but with a smaller thread id have finished
        while ((for every other thread that is NOT thread_id) (level[j] >= i && victim[i] == thread_id)) {};
    }
}
```

### Unlock
The `unlock()` function will simply reset the level of the thread to 0.

**Pseudocode**
```cpp
void unlock(int thread_id) {
    level[thread_id] = 0;
}
```

## Lamport's Bakery Algorithm
Lamport's Bakery Algorithm is another way to implement a mutex. However, unlike the Filter Lock, it guarantees *first-come-first-served* property by using a distributed version of a ticket dispenser.

Each thread will takes a ticker (or number in the doorway), then waits until no thread with an earlier ticket(number) is trying to enter the critical section.

### Constructor & Initialization
Similar to the filter lock, two arrays are going to be needed: number and entering. The number array will be tracking the ticket number of each thread. Meanwhile, the entering is needed to indicate whether a thread is trying to get a ticket. (Ensures that a thread does not get a ticket while another thread is trying to get one.)

**Pseudocode**
```cpp
class mutex {
 public:
  mutex() {} // constructor is not needed
  
  void init (int num_threads) {
    _num_threads = num_threads;
    entering.resize(num_threads, false);
    number.resize(num_threads, 0);
  }

 private:
    vector<atomic<bool>> entering;
    vector<atomic<int>> number;
    int _num_threads;
};
```

### Lock
The `lock()` function will first set the entering flag to true, then set the number of the current thread to max of all other threads' numbers + 1. The entering flag will be set to false once the thread has received its ticket number. The thread will then wait until all other threads have received their ticket number and all threads with smaller numbers have finished their work. 

Side note: A ticket variable to keep track of the current maximum ticket number could be implemented to avoid the max function if RMW were allowed.


**Pseudocode**

```cpp
void lock(int thread_id) {
    entering[thread_id] = true;
    number[thread_id] = 1 + max(number[0], number[1], ..., number[_num_threads - 1]);
    entering[thread_id] = false;

    for(int j = 1; j <= _num_threads; ++j){
        // wait until thread j receives its number
        while(entering[j]) {};

        // wait until all threads with smllaer numbers finish their work; meanwhile spin
        while((number[j] != 0) && ((number[j], j) < (number[thread_id], thread_id))) {};
    }
}
```

### Unlock
The `unlock()` function will simply reset the number of the thread to 0.

**Pseudocode**
```cpp
void unlock(int thread_id) {
    number[thread_id] = 0;
}
```


