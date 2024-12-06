#include <atomic>

using namespace std;

class dekkers_mutex {
  public:
    dekkers_mutex() {
        // implement me!
        flag[0].store(false, memory_order_seq_cst);
        flag[1].store(false, memory_order_seq_cst);
        turn.store(0, memory_order_seq_cst);
    }

    void lock(int tid) {
        // implement me!
        int other = 1 - tid; // The other thread's tid

        flag[tid].store(true, memory_order_seq_cst);

        // Wait until the other thread is done
        while (flag[other].load(memory_order_seq_cst)) {
            if (turn.load(memory_order_seq_cst) != tid) {
                flag[tid].store(false, memory_order_seq_cst);
                while (turn.load(memory_order_seq_cst) != tid)
                    ;
                flag[tid].store(true, memory_order_seq_cst);
            }
        }
    }

    void unlock(int tid) {
        // implement me!
        flag[tid].store(false, memory_order_seq_cst);
        turn.store(1 - tid, memory_order_seq_cst); // Give the other thread a chance
    }

  private:
    // Give me some private variables
    atomic<bool> flag[2];
    atomic<int> turn;
};
