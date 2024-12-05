#include <atomic>

using namespace std;

// this should be the same as SCDekkers with the change that every
// relaxed access should be memory_order_relaxed

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

        // Wait until the other thread is done
        while (flag[other].load(memory_order_relaxed)) {
            if (turn.load(memory_order_relaxed) != tid) {
                flag[tid].store(false, memory_order_relaxed);
                while (turn.load(memory_order_relaxed) != tid)
                    ;
                flag[tid].store(true, memory_order_relaxed);
            }
        }
    }

    void unlock(int tid) {
        // implement me!
        flag[tid].store(false, memory_order_relaxed);
        turn.store(1 - tid, memory_order_relaxed); // Give the other thread a chance
    }

  private:
    // Give me some private variables
    atomic<bool> flag[2];
    atomic<int> turn;
};
