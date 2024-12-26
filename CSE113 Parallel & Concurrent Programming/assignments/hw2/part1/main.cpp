#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

using namespace std::chrono;

#define SECONDS 1

enum MutexType { cpp, filter, bakery };

// Pull in different mutex implementations
#if defined(CPP_MUTEX)
#include <mutex>

std::mutex m;
MutexType t = cpp;
#define init(n) 
#define lock(t) m.lock()
#define unlock(t) m.unlock()

#elif defined(FILTER_MUTEX)

#include "filter.h"
mutex m;
MutexType t = filter;
#define init(n) m.init(n);
#define lock(t) m.lock(t);
#define unlock(t) m.unlock(t);

#elif defined(BAKERY_MUTEX)

#include "bakery.h"
mutex m;
MutexType t = bakery;
#define init(n) m.init(n);
#define lock(t) m.lock(t);
#define unlock(t) m.unlock(t);

#else
#error No mutex specified!
#endif

int total_number = 0;
int warmup_number = 0;
int *histogram;

// runs for 1 second, locking and unlocking over and over
void test_mutex(int thread_id) {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    
    // Perform the lock and unlock
    lock(thread_id);

    if (warmup_number < 501) {
      warmup_number++;
      if (warmup_number % 100 == 0) {
        std::this_thread::sleep_for(nanoseconds(1));
      }
    } else {
      total_number++;
      histogram[thread_id]++;
    }
    // if (warmup_number % 150 == 0 or total_number % 150 == 0) {
    //   std::this_thread::sleep_for(nanoseconds(1));
    // }
    unlock(thread_id);
    
    // Update the timer
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }
}

// Function to set CPU affinity for a thread
void set_affinity(std::thread &thread, int core_id) {
  pthread_t native_handle = thread.native_handle();
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset);
}

int main(int argc, char *argv[]) {

  // Read the number of threads in from the command line
  int num_threads = 8;
  if (argc == 2) {
    num_threads = atoi(argv[1]);   
  }

  // Create a new array for the number of threads
  std::thread *thread_array = new std::thread[num_threads];
  init(num_threads);

  // Initialize the histogram
  histogram = new int[num_threads];
  for (int i = 0; i < num_threads; i++) {
    histogram[i] = 0;
  }

  // Launch the threads
  // for (int i = 0; i < num_threads; i++) {
  //   thread_array[i] = std::thread(test_mutex, i);
  // }
  for (int i = 0; i < num_threads; i++) {
    thread_array[i] = std::thread(test_mutex, i);
    // Pin each thread to a specific core
    set_affinity(thread_array[i], i);
  }

  // Join the threads
  for (int i = 0; i < num_threads; i++) {
    thread_array[i].join();
  }

  // Print stats
  std::cout << "total number: " << total_number << std::endl;

  for (int i = 0; i < num_threads; i++) {
    std::cout << histogram[i] << std::endl;
  }

  // cleanup the memory
  delete[] thread_array;
  delete[] histogram;
}
