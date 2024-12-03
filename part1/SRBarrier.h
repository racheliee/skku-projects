#include <atomic>

using namespace std;

class barrier_object {
  public:
    barrier_object() {
        // Probably don't need to do anything here.
    }

    void init(int num_threads) {
        // Implement me
        size = num_threads;
        counter.store(num_threads);
        sense.store(false);
    }

    void barrier(int tid) {
        // Implement me
        static thread_local bool my_sense = !sense.load();
        int pos = counter.fetch_sub(1);

        if (pos == 1) {
            counter.store(size);
            sense.store(my_sense);
        } else {
            while (sense.load() != my_sense)
                ;
        }

        my_sense = !my_sense;
    }

  private:
    // Give me some private variables
    atomic<int> counter;
    int size;
    atomic<bool> sense;
};
