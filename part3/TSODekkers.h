#include <atomic>

// this should be the same as RDekkers with the change that every
// relaxed access should be memory_order_relaxed.
// your job is to use the following MACRO to fix the mutex.
// Think about the reorderings allowed under TSO and where the fence
// needs to be placed.

#define FENCE asm volatile("mfence" ::: "memory");

using namespace std;

class dekkers_mutex {
  public:
    dekkers_mutex() {
        // implement me!
        flag[0].store(false, memory_order_relaxed);
        flag[1].store(false, memory_order_relaxed);
        turn.store(0, memory_order_relaxed);
    }

    void lock(int tid) {
        // implement me!
        int other = 1 - tid; // The other thread's tid

        flag[tid].store(true, memory_order_relaxed);
        FENCE; // ensure flag store is visible to other threads

        // Wait until the other thread is done
        while (flag[other].load(memory_order_relaxed) && turn.load(memory_order_relaxed) == tid)
            ;
    }

    void
    unlock(int tid) {
        // implement me!
        flag[tid].store(false, memory_order_relaxed);
        FENCE; // ensure flag store is visible to other threads
        turn.store(1 - tid, memory_order_relaxed); // Give the other thread a chance
    }

  private:
    // Give me some private variables
    atomic<bool> flag[2];
    atomic<int> turn;
};
