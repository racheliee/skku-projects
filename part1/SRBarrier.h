#include <atomic>

using namespace std;

class barrier_object {
 public:
  barrier_object() : count(0), num_threads(0), sense(true) {
    // Probably don't need to do anything here.
  }

  void init(int num_threads) {
    // Implement me
    this->num_threads = num_threads;
    count.store(0);
    sense.store(true);
  }

  void barrier(int tid) {
    // Implement me
    static thread_local bool thread_sense = !sense.load();
    thread_sense = !thread_sense;

    int pos = count.fetch_add(1);
    if(pos == num_threads -1) {
      count.store(0);
      sense.store(thread_sense);
    }else{
      while(sense.load() != thread_sense);
    }
  }

private:
  // Give me some private variables
  atomic<int>  count;
  int num_threads;
  atomic<bool> sense;
};
