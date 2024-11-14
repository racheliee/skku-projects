#include <math.h>
#include <thread>
#include <iostream>
#include <chrono>
#include "checker.h"

#if defined(PART1)
#include "CQueueSync.h"
QueueType t = sync;
#elif defined(PART2)
#include "CQueue.h"
QueueType t = async;
#elif defined(PART3)
#include "CQueue.h"
QueueType t = eight_chunked;
#endif

CQueue memory_to_trig;
CQueue trig_to_memory;

#if defined(PART3)
#include "part3_thread_funcs.h"
#else
void memory_load_thread_func(float *a, int size) {

  for (int i = 0; i < size; i++) {
    float loaded = a[i];
    memory_to_trig.enq(loaded);
  }
}

void trig_thread_func(int size) {

  for (int i = 0; i < size; i++) {
    float consume = memory_to_trig.deq();
    float to_store = cos(consume);
    trig_to_memory.enq(to_store);

  }
}

void memory_store_thread_func(float *a, int size) {

  for (int i = 0; i < size; i++) {
    float to_store = trig_to_memory.deq();
    a[i] = to_store;    
  }
}
#endif

int main() {
  float *a = new float[SIZE];
  for (int i = 0; i < SIZE; i++) {
    a[i] = 0.5 + i;
  }
  auto start = std::chrono::high_resolution_clock::now();

  std::thread memory_load_thread(memory_load_thread_func,a,SIZE);
  std::thread trig_thread(trig_thread_func, SIZE);
  std::thread memory_store_thread(memory_store_thread_func,a,SIZE);

  memory_load_thread.join();
  trig_thread.join();
  memory_store_thread.join();

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  double seconds = duration.count()/1000000000.0;
  check_results(a, SIZE, t);
  check_timing(seconds, t);

  std::cout << "time (seconds): " << seconds << std::endl;
 
  return 0;
}
