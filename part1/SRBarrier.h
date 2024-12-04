#include <atomic>

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
        bool old_flag = sense.load();
        int pos = counter.fetch_add(1); 

        if (pos == num_threads - 1) {
            counter.store(0);
            sense.store(!old_flag);
        } else {
            while (sense.load() == old_flag);
        }

    }

  private:
    // Give me some private variables
    int num_threads;
    atomic<int> counter;
    atomic<bool> sense;
};
