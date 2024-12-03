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
    }

    void barrier(int tid) {
        // Implement me
        static thread_local bool my_sense = !sense.load();
        my_sense = !my_sense;
        int arrival = counter.fetch_add(1) + 1;

        if (arrival == num_threads) {
            counter.store(0);
            sense.store(my_sense);
        } else {
            while (sense.load() != my_sense)
                ;
        }

    }

  private:
    // Give me some private variables
    atomic<int> counter;
    int num_threads;
    atomic<bool> sense;
};
