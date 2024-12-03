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
    count.store(num_threads);
    sense.store(false);
  }

  void barrier(int tid) {
    // Implement me
    static thread_local bool my_sense = !sense.load();
    int pos = count.fetch_add(-1);

    if (pos == 1) {
      count.store(size);
      sense.store(my_sense);
    }else{
      while (sense.load() != my_sense);
    }

    my_sense = !my_sense;
  }

private:
  // Give me some private variables
  atomic<int> count;
  int size;
  atomic<bool> sense;
};
