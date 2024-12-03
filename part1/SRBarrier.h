#include <atomic>
#include <vector>

using namespace std;

class barrier_object {
  public:
    barrier_object() {
        // Probably don't need to do anything here.
    }

    void init(int num_threads) {
        // Implement me
        this->num_threads = num_threads;
        counter.store(0);
        sense.store(false);
        thread_sense.resize(num_threads, true);
    }

    void barrier(int tid) {
        // Implement me
        int arrival = counter.fetch_add(1) + 1;
        if (arrival == num_threads) {
            counter.store(0);
            sense = thread_sense[tid];
        } else {
            while (sense != thread_sense[tid])
                ;
        }

        thread_sense[tid] = !thread_sense[tid];
    }

  private:
    // Give me some private variables
    atomic<int> counter;
    int num_threads;
    atomic<bool> sense;
    vector<bool> thread_sense;
};
