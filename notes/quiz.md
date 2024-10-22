# Quiz reviews

## quiz #2

1. How many instructions will `z = x + x + x + x` translate to in C or Java?
   - 2 (uses multiplication and store)
2. How many levels of cache does x86 have?
   - 3
3. Why is it difficult ot reason about program performance when using a high-level language like python?
   - abstraction is a good thing. it may help in optimization overall
   - python is slower due to an interpreter
4. How much faster does C/java run compared to python?
   - python is two orders of magnitude slower
   - c is a very unsafe language: writing out of bounds, etc.
     - has no runtime checks to reduce overhead
     - you can access objects that are not in the array
5. programmers do not need to think about parallelism when writing programs bc modern-day compilers runtime will automatically make your code in parallel
   - false

## quiz #3

1. How many elements of type double can be stored in a cache line?
   - Intel: `64B / 8B = 8`
   - Apple: `128B / 8B = 16`
2. Instructions with the following property should be placed as far apart as possible in machine code:

   - [ ] Instructions that compute floating point values
   - [ ] Instructions that load from memory => 앤 아닌건가???
   - [x] Instructions that depend on each other
     - ILP can run them in parallel if separated
   - [ ] Instructions that perform the same operation

3. What does ILP stand for?

   - Instruction Level Parallelism

4. C++ threads are initialized with a function argument where they will start execution, but they must be explicitly started with the "launch" command.

   - false
   - immediately starts after creation (in java you need to launch after creation)

5. The "join" function for a C++ thread causes the thread to immediately exit.

   - false
   - waits for the thread to finish

6. A thread that is launched will eventually exit by itself and there is no need for the main thread to keep track of the threads it launches.

   - false
   - runtime error if the threads are still running after the main thread exits

7. In 2 or 3 sentences, explain the difference between instruction level parallelism and thread parallelism
   - instruction level parallelism
     - multiple instructions are executed at the same time in the same thread
     - there is a single thread of execution of a process
     - low level parallelism
   - thread parallelism: multiple threads are executed at the same time
     - there is separate flow of control to each worker
     - high level parallelism

## quiz #4

1. It is possible to interleave the load and store operations of RMW atomic operations; however, it is so rare that it does not matter in practice.
   - false
   - "possible to interleave the load and store operations of RMW atomic operations" => 이러면 atomic하지 않음
   - "it is so rare that it does not matter in practice" => dangerouss
2. A data conflict is when two threads access the same memory location.
   - false
   - 둘중 하나라도 write를 해야 data conflict가 발생; 그냥 read면 괜찮음
3. How many interleavings are possible with 3 threads, each them executing 1 event?
   - 6
   - (3 choose 2) = 3! = 6
4. How many extra arguments are required to turn a function into an SPMD function?
   - 2
   - tid, num_threads
5. Write a few sentences about how you can remove data-conflicts from your program. We have mentioned a few ways in class, but feel free to mention other ways you can think of!
   - use atomic instructions
   - use mutexes, semaphores, or other sorts of locks

## quiz #5
1. Which one of the answers is NOT a property of mutexes?
   - [ ] Deadlock Freedom
   - [ ] Mutual Exclusion
   - [x] Deterministic Execution
   - [ ] Starvation Freedom
2. We should aim to make mutual exclusion regions as short as possible because of the caching overhead of locks.
   - True
3. If you run your code with the thread sanitizer and if it doesn't report any issues, then your code is guaranteed to be free from data-conflicts
   - False
     - instruction level에서 생기는 문제도 있지 않나...?
   - thread sanitizer is a tool that detects data races through compiler instrumentation and runtime checking. Typical memory overhead introduced by ThreadSanitizer is about 5x-10x.
4. It is required to use atomic types inside of critical sections 
   - False
     - critical section을 보호하기 위해 atomic type을 사용하는거 아닌가
5. Write 1 or 2 sentences about whether you agree or disagree with the following sentence and why:
   >"Because atomic data types can safely be accessed concurrently, we should mark all our variables as atomic just to be safe."
   - I disagree with the sentence because while atomic data types provides protection from data conflicts and other synchronization issues, it also introduces significant overhead as it constantly accesses the main memory. Furthermore, making all variables atomic would take away the benefits of concurrency since threads will need to wait to access the variables; i.e. scalability is decreased.
6. Write a few sentences about how you can reason about the correctness of a mutex implementation.
   - To reason the correctness of a mutex, consider if it provides mutual exclusion, deadlock freedom, and starvation freedom. Furthermore, in order to prevent deadlocks check if the deadlock conditions: mutual exclusion, circular wait, no preemption, and hold and wait. Lastly, to ensure fairness implement techniques that will bound the wait time of the threads, such as ticket locks.

## quiz #6
1. What happens when two atomic store operations write to the same location at the same time
with different values?
   - [ ] This is a data conflict and should be avoided
   - [ ] It is undefined behavior and the memory location is allowed to contain any possible value
   - [x] The value from one of the threads will be stored in the location
   - [ ] Each thread will store their value in their cache and they will be able to read this value later on
2. What does a C++ RMW operation return?
   - [x] a boolean indicating whether it succeeded or not
   - [ ] the value after the modification
   - [x] the value before the modification
   - [ ] nothing, however it is guaranteed that the modification occurred atomically (indivisibly) in memory
3. What is the difference between an atomic exchange and an atomic compare and swap?
   - both are rmw modify operations
   - exchange: always writes a value to the memory location
   - compare and swap: writes a value to the memory location only if the current value is equal to the expected value
4. CAS and Exchange locks are not starvation free, but starvation is so rare that it does not matter in practice
   - both
   - preemption이 있기 때문에 starvation이 실제로 발생하는 경우는 적음
   - 그렇다고 고려 안하는것 좋지 않음
5. Which of the following locks have required a RMW atomic for unlocking?
   - [ ] CAS lock
   - [ ] Exchange lock
   - [ ] Ticket lock
   - [ ] all of the above
   - [x] none of the above
6. discuss some of the trade-offs between a fair mutex and unfair mutex
   - 
7. Discuss a few trade-offs between RMW mutexes and the simpler load/store mutexes (e.g. peterson's lock).
   - RMW can scale better and show better performance even with contention
   - simple load/store mutexes can be used in situations when there's no hardware support for RMW
     - fundamentally important
     - shows mutual exclusion can be achieved even without RMW

## quiz #7
1. Which of the following are NOT ways that mutex implementations can encourage fair access?
   - [ ] Sleeping
   - [ ] Yielding
   - [ ] Using a ticket lock
   - [ ] relaxed peeking
2. A reader-writer mutex allows multiple readers in the critical section, mutliple writers in the critical section, but never a combination of readers and writers.
   - false
   - multiple readers or single writer at a time
3. If you are an expert in how your code will compile to machine instructions, it is okay to have data conflicts in your code.
   - false
4. Why is the compare-and-swap operation required after the relaxed peeking sees that the mutex is available?
   - compare-and-swap is required to ensure that the mutex is still available when the thread tries to acquire the lock. relaxed peeking is not enough to guarantee that the lock is still available because the state of the lock can change between the time the thread sees the lock is available and the time it tries to acquire the lock.