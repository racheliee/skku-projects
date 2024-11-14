ScheduleType t = local_32_t;

#include <atomic>
#include <thread>
#include <vector>
#include "IOQueue.h"

using namespace std;

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
  // Use this function to equeue indexes to each of the local
  // workstealing queues

  // Hint: this should be the same as in main3.cpp
  int chunk = size/num_threads;
  int start = tid*chunk;
  int end = (tid == num_threads-1) ? size : start + chunk;

  for (int i = start; i < end; ++i) {
    Q[tid].enq(i); // enqueue the index
  }
}

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Implement me using a workstealing approach.
  // You should use the Q IOQueues to compute
  // local work and then try to steal work
  // from others until all the work is completed

  // Unlike in part3_local.h, you should deq 32 elements
  // at a time.

  int tasks[32];

  // Process tasks in chunks of 32 from the local queue
  while (Q[tid].deq_32(tasks) == 0) {
    for (int k = 0; k < 32; k++) {
      int task = tasks[k];
      float base = result[task];
      for (int j = 0; j < mult[task] - 1; j++) {
        result[task] += base;
      }
    }
  }

  // Work stealing loop for stealing chunks of 32 tasks
  while (true) {
    bool has_work = false;

    // Check if all threads are finished
    if (finished_threads.load() >= num_threads) {
      break;
    }

    // Attempt to steal a chunk of 32 tasks from other queues
    for (int i = 0; i < num_threads; ++i) {
      if (i != tid && Q[i].deq_32(tasks) == 0) {  // Successful steal
        for (int k = 0; k < 32; k++) {
          int task = tasks[k];
          float base = result[task];
          for (int j = 0; j < mult[task] - 1; j++) {
            result[task] += base;
          }
        }
        has_work = true;
        break;
      }
    }

    if (!has_work) {
      // No work found in any queue, mark thread as finished
      atomic_fetch_add(&finished_threads, 1);
      break;
    }
  }

}

void launch_threads(float* result_parallel, int* mult) {
  // Launch NUM_THREADS threads to intialize the IOQueues in Q with
  // the indexes for each thread. Remember to initialize the Queues
  // with the size that they will need. Join the threads afterwards.

  // Next, launch the parallel function that performs the parallel_mult
  // function from part1_static.h and part2_global.h but using workstealing

  // hint: this part should be the same as in part3_local.h

  vector<thread> threads;
  for(int i = 0; i < NUM_THREADS; i++) {
    Q[i].init(SIZE);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_enq, SIZE, i, NUM_THREADS);
  }

  for(auto& thread : threads) {
    thread.join();
  }

  threads.clear();

  finished_threads.store(0);

  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  for(auto& thread : threads) {
    thread.join();
  }
}