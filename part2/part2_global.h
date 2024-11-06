#include <thread>
#include <vector>

using namespace std;

ScheduleType t = global_t;

#include <atomic>
std::atomic_int counter(0);

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Write me a parallel way using the global counter
  // as an implicit worklist. That is, every thread
  // should get its index from incrementing the counter
  // for (int i = 0; i < size; i++) {
  //   float base = result[i];
  //   for (int w = 0; w < mult[i]-1; w++) {
  //     result[i] = result[i]+base;
  //   }
  // }

  while(1){
    int i = counter.fetch_add(1);
    if(i >= size){
      break;
    }

    float base = result[i];
    for (int w = 0; w < mult[i]-1; w++) {
      result[i] = result[i]+base;
    }
  }
}

void launch_threads(float* result_parallel, int* mult) {
  // Launch NUM_THREADS threads and then join them

  vector<thread> threads;

  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  for(auto& thread : threads) {
    thread.join();
  }
}