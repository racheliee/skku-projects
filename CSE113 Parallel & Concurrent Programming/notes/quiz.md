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
   - [x] relaxed peeking
2. A reader-writer mutex allows multiple readers in the critical section, mutliple writers in the critical section, but never a combination of readers and writers.
   - false
   - multiple readers or single writer at a time
3. If you are an expert in how your code will compile to machine instructions, it is okay to have data conflicts in your code.
   - false
4. Why is the compare-and-swap operation required after the relaxed peeking sees that the mutex is available?
   - compare-and-swap is required to ensure that the mutex is still available when the thread tries to acquire the lock. relaxed peeking is not enough to guarantee that the lock is still available because the state of the lock can change between the time the thread sees the lock is available and the time it tries to acquire the lock.
 
## quiz #8
1. It is impossible to use objects that are not thread-safe in a concurrent program.
   - false
   - you can use locks to protect the data structures
2. Non-locking objects do not use mutexes in their implementation. This is beneficial because:
   - [x] it is faster
   - [ ] it is easier to reason about
   - [x] it is easier to extend
3. When multiple threads access a concurrent object, only 1 possible execution is allowed. We reason about that execution by sequentializing object method calls and it is called sequential consistency
   - false
   - this is called linearizability
     - each method call on a concurrent object appears to take effect instantaneously at some point between its invocation and its response, ensuring that even though multiple threads access the object, only one possible execution is valid
   - Sequential consistency, on the other hand, ensures that operations of all threads are interleaved in a single order, but this order does not have to respect the real-time order of operations. In other words, sequential consistency guarantees a total order of operations that could be arbitrary, as long as it’s consistent across threads.
4. What is the relationship between linearizable (L) and sequentially consistent (SC)?
   - Objects that are L are also SC, but not the other way around.
5. Write a few sentences about the pros and cons of using a concurrent data structure vs. using mutexes to protect data structures that are not thread-safe.
   - pro: easier to use // con: composability
   - Using concurrent data structures is beneficial because it allows for multi-threaded programming without the concern for locks when accessing or modifying the data structure thanks to its internal synchronization mechanisms. However, the concurrent data structures are compplex and could potentially present difficulty in debugging.
   - Mutexes provide flexibility in protecting data structures. A user can declare their own data structures and make them thread-safe through mutexes. However, this approach can lead to significant performance drawbacks due to increased blocking and potential contention, as threads must wait for exclusive access to the resource, which can lead to issues like deadlocks and priority inversion if not managed carefully.


## quiz #9
1. Input/output queues use atomic increments and decrements to protect against threads that are trying to concurrently enqueue and dequeue
   - true
   - atomic increments and decrements are used to ensure that the producer and consumer threads do not access the same memory location at the same time
2. The performance of an application using a producer-consumer queue depends most on:
   - [ ] If the queue is implemented using mutex or not
   - [ ] The rate at which the consumer enqueues elements --> consumer dequeues
   - [x] The rate at which the producer enqueues elements
3. A circular buffer is:
   - [x] A useful data representation for fixed-length queues
   - [ ] Part of the C++ standard library
   - [ ] A special type of memory that is organized in circular pattersTrue
4. Lock-free data structures are technically undefined because they contain data conflicts
   - false
   - lock-free data structures are defined as data structures that can be accessed by multiple threads without the use of locks
5. Write a few sentences about the pros and cons of using a specialized concurrent queue (e.g. an IO queue) and a fully general concurrent queue.
   - A specialized concurrent queue offers optimized performance for specific tasks (e.g., I/O operations) but lacks flexibility for other workloads. A general concurrent queue is versatile and reusable but may have lower performance due to lack of specific optimizations.

## quiz #10
1. A DOALL Loop must have:
   - [ ] A loop variable that starts at 0 and is incremented by 1
   - [x] loop iterations that are independent
   - [ ] be unrolled and interleaved
   - [ ] not access any memory locations
2. Given what we've learned: what role do you believe the compiler should play in parallelizing DOALL loops? For example, should it: (1) identify them? (2) parallelize them? (3) pick a parallel schedule? There is no right or wrong answer here, but it is interesting to think about!
   - (1) identify doall loops 
   - (2) parallelize them to convert the identified doall loops into parallel code
   - (3) pick a parallel schedule to optimize the performance of the parallelized code
3. Which one of the following is NOT a drawback of a global workstealing parallel schedule
   - [x] Requires a concurrent data structure
   - [ ] Contention on shared cache lines
   - [ ] Contention on a single location with RMWs
4. Which of the following is NOT an overhead of the local worklist workstealing parallel schedule (that we studied in class)
   - [ ] Initialization of the queues
   - [ ] Checking a global variable to ensure all work is completed
   - [x] Managing concurrent enqueues to the worklists
5. Which of the following solutions can guarantee that a static schedule will not be out of bounds?
   - [x] The last thread always checks to get the minimum between the end of the array or the value allocated
   - [ ] The last thread always get the end of the array
   - [ ] The last thread never receives more than N tasks
6. Write a few sentences about the pros and cons of using local workstealing queues over the global implicit worklist
   - simple vs more efficient
   - pros:
     - reduced contention because local workstealing queues minimize contention on shared data structures
     - better cache performance because each thread has its own queue
     - better scalability because threads can work independently
   - cons:
     - load imbalance could occur because threads may not have the same amount of work
     - overhead of work stealing because threads need to check other threads' queues
     - more complex to implement because each thread has its own queue


## quiz #11
1. The host (CPU) will write a C++-like program that allocates and sets up memory on the GPU. The host will then call a GPU program called a kernel. Is this affirmation true?
   - true
2. How do we initialize memory for a variable we aim to use in the GPU computation?
   - [x] Using cudaMemcpy
   - [ ] Using memcpy from C++
   - [ ] Just declaring a new variable
3. What keyword do we need to include for a function to be executed on the GPU using CUDA?
   - `__kernel__`
4. example of workloads that benefit from GPU parallelism
   - data-parallel workloads
   - image processing, scientific simulations, machine learning, and deep learning


## quiz #12
1. What is a warp?
   - [x] Group of 32 threads in a GPU
   - [ ] Group of 16 threads in a GPU
   - [ ] Group of 2 threads in a GPU
   - [x] Group of some threads in a GPU --> 여러개 일 수도 있으니까!! spectrum of designs
2. Like the CPU cache, the Load/Store Unit reads in memory in chunks. Is this affirmation true or false?
   - true
   - the Load/Store Unit reads in memory in chunks, just like the CPU cache
3. What we could observe with the demonstrations made in class about memory accesses on GPUs?
   - Memory coalescing accelerates execution
   - In class, we saw that how data is accessed in GPU memory can make a big difference in performance. When threads access memory locations in order, the GPU can handle it more efficiently, reducing the time it takes. But if threads access random spots in memory, it slows things down because the GPU has to do more work to get the data. Using shared memory, where threads in a block can share data, also helps speed things up by reducing the need to access slower global memory.
4. Why do we need to calculate int i = blockIdx.x * blockDim.x + threadIdx.x?
   - [ ] To get the index in the matrix we want to compute in that thread
   - [x] To get the whole matrix we want to compute in that thread
   - [ ] To get the index in the matrix we want to compute in that warp


## quiz #13
1. A relaxed memory execution refers to:
   - [ ] An execution where some stores failed to reach main memory
   - [ ] Any execution which contains a data-conflict
   - [ ] An execution that utilizes the processor's store buffer
   - [x] An execution that is not sequentially consistent
2. Which of the following memory accesses pairs, when they appear in program order, does x86 allow to be re-ordered?
   - [ ] Load followed by a Store
   - [ ] Load followed by a Load
   - [ ] Store followed by a Store
   - [x] Store followed by a Load
3. How do weak memory models cause unintended behaviors in parallel code?
   - Weak memory models allow memory operations to be reordered or delayed, leading to inconsistencies between the order of operations intended by the programmer and the order observed by threads. This can cause unintended behaviors in parallel code, such as race conditions, stale or incorrect data being read, and violations of dependencies between threads, ultimately leading to hard-to-debug errors.

## quiz #14
1. The C++ relaxed memory order provides
   - [ ] no orderings at all
   - [x] orderings only between accesses of the same address
   - [ ] TSO memory behaviors when run on an x86 system
   - [ ] an easy way to accidentally introduce horrible bugs into your program
2. In terms of memory models, the compiler needs to ensure the following property:
   - [ ] Any weak behavior allowed in the language is also allowed in the ISA
   - [x] Any weak behaviors that are disallowed in the language need to be disallowed in the ISA
   - [ ] The compilation ensures that the program has sequentially consistent behavior at the ISA level
   - [ ] The compiler does not need to reason about relaxed memory
3. A program that uses mutexes and has no data conflicts does not have weak memory behaviors for which of the following reasons?
   - [ ] Mutexes prevent memory accesses from happening close enough in time for weak behaviors to occur
   - [ ] The OS has built in support for Mutexes that disable architecture features, such as the store buffer
   - [x] A correct mutex implementation uses fences in lock and unlock to disallow weak behaviors
4. Assuming you had a sequentially consistent processor, any C/++ program you ran on it would also be sequentially consistent, regardless of if there are data-conflicts or not.
   - false
   - compiler can reorder instructions
5. If you put a fence after every memory instruction, would that be sufficient to disallow all weak behaviors on a weak architecture? Please write a few sentences explaining your answer.
   - sufficient but slow
   - Yes, placing a fence after every memory instruction would disallow all weak behaviors because fences enforce ordering constraints and flush store buffers. However, this approach is inefficient, as it would significantly degrade performance by preventing hardware optimizations like instruction reordering and store buffering, which are the main benefits of weak memory models.

## quiz #15
1. Concurrent linked lists can be implemented using locks on every node if:
   - [x] locks are always acquired in the same order
   - [ ] two locks are acquired at a time
   - [ ] Both of the above
   - [ ] Neither of the above
2. Lock coupling provides higher performance than a single global lock because threads can traverse the list in parallel 
   - true
   - Lock coupling allows fine-grained locking, enabling threads to operate on different parts of the list concurrently, improving performance compared to a single global lock
3. Optimistic concurrency refers to the pattern where functions optimistically assume that no other thread will interfere. In the case where another thread interferes, the program is left in an erroneous state, but since this is so rare, it does not tend to happen in practice.
   - false
   - In optimistic concurrency, functions assume no interference, but if interference is detected (via validation or retries), the operation is retried or aborted to maintain correctness. The program should never be left in an erroneous state
4. After this lecture, do you think you would be able to optimize your implementation of the concurrent stack in homework 2? Write a few sentences on what you might try.
   - Yes, I think I could optimize my implementation. For instance, I might explore using lock-free techniques, such as compare-and-swap (CAS), to reduce contention and avoid blocking other threads. Additionally, I could consider employing fine-grained locking or a backoff strategy to minimize lock contention and improve scalability. Finally, leveraging ideas from optimistic concurrency could also allow threads to operate speculatively and retry only when conflicts occur.

## quiz #16
1. Barrier objects have how many API calls?
   - 2
   - barrier_wait, barrier_init
2. A barrier call emits which of the following events? Check all that apply
   - [ ] barrier_lock
   - [x] barrier_arrive
   - [ ] barrier_enqueue
   - [x] barrier_leave
3. If a program uses both barriers and mutexes, the outcome is deterministic (i.e. the same every time) if there are no data conflicts
   - true
   - If there are no data conflicts and synchronization primitives (barriers and mutexes) are used correctly, the program's execution order is deterministic. Synchronization ensures predictable thread interactions
4. Write a few sentences about what you think the best interface for parallel programming is: that is: do you think it is atomics? Mutexes? Concurrent Data Structures? Barriers? Or even maybe the compiler should simply do it all automatically? Or is it some combination of the above? What are the trade-offs involved?
   - The best interface for parallel programming depends on the specific requirements and trade-offs of the problem at hand. Low-level primitives like atomics are lightweight and highly efficient but can be error-prone due to their complexity in ensuring correctness. Mutexes are effective for protecting shared resources, but their usage introduces overhead and risks deadlocks if not managed carefully. Concurrent data structures abstract much of the complexity, allowing safe concurrent access, though they may have higher overhead compared to hand-optimized solutions. Barriers are ideal for synchronizing threads in phases but lack versatility for finer-grained control. Compiler automation offers a promising way to offload the complexity of parallel programming but may lack transparency or flexibility in handling complex systems. Ultimately, a combination of these approaches often yields the best results: higher-level abstractions and automation for productivity and simplicity, coupled with lower-level primitives for performance-critical sections. The trade-offs involve balancing efficiency, ease of use, control, and complexity.
