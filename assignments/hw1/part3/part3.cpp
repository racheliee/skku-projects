#include <thread>
#include <vector>

using namespace std;

/** You may add additional functions as needed in this file. */

/** Write your sequential computation here. */
void sequential_increment(volatile int *a, int size, int k) {
  // Implement me!
  for(int i = 0; i < size; ++i){
    for(int j = 0; j < k; ++j){
      a[i]++;
    }
  }
}

/** Write your round robin computation here.*/
void round_robin_increment(volatile int *b, int size, int k, int num_threads) {
  // Implement me!
  // lambda function which increments the array based on the thread id
  auto increment = [b, size, k, num_threads](int tid) {
    for(int i = tid; i < size; i+= num_threads){
      for(int j = 0; j < k; ++j) {
        b[i]++;
      }
    }
  };

  // create threads
  std::vector<thread> t;
  for (int i = 0; i < num_threads; ++i) {
    t.push_back(thread(increment, i));
  }
  

  // join threads
  for (int i = 0; i < num_threads; ++i) {
    t[i].join();
  }
}


/** Write your custom computation here. */
void custom_increment(volatile int *c, int size, int k, int num_threads) {
  // Implement me!
  // lambda function which increments the array based on the thread id
  auto increment = [c, size, k, num_threads](int tid) {
    int end = min(size, size/num_threads * (tid + 1));
    for(int i = size/num_threads *tid; i < end; ++i){
      for(int j = 0; j < k; ++j) {
        c[i]++;
      }
    }
  };

  // create threads
  std::vector<thread> t;
  for (int i = 0; i < num_threads; ++i) {
    t.push_back(thread(increment, i));
  }
  

  // join threads
  for (int i = 0; i < num_threads; ++i) {
    t[i].join();
  }
}
