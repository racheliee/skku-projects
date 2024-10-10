#pragma once

#include <atomic>

class mutex {
 public:
  mutex() {
    // Implement me!
  }
  
  void init (int num_threads) {
    // Implement me!
  }
  
  void lock(int thread_id) {
    // Implement me!
  }
  
  void unlock(int thread_id) {
    // Implement me!
  }

 private:
  // Give me some private variables!
};
