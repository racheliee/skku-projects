#include <atomic>

class barrier_object {
 public:
  barrier_object() {
    // Probably don't need to do anything here.
  }

  void init(int num_threads) {
    // Implement me
  }

  void barrier(int tid) {
    // Implement me
  }

private:
  // Give me some private variables
};
