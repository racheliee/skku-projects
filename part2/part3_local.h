#include <thread>
#include <vector>
#include <atomic>
#include "IOQueue.h"

using namespace std;

ScheduleType t = local_t;

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
  // Initialize each thread's queue with indices assigned in a chunked style
  int chunk = size / num_threads;
  int start = tid * chunk;
  int end = (tid == num_threads - 1) ? size : start + chunk;

  for (int i = start; i < end; ++i) {
    Q[tid].enq(i);  // Enqueue the index into the local queue
  }
}

void parallel_mult(float *result, int *mult, int size, int tid, int num_threads) {
  while (true) {
    int idx = Q[tid].deq();  // Attempt to dequeue from the local queue

    // If local queue is empty, attempt to steal from other queues
    if (idx == -1) {
      bool success = false;
      for (int i = 0; i < num_threads; i++) {
        if (i != tid) {
          idx = Q[i].deq();  // Try stealing from queue of thread `i`
          if (idx != -1) {
            success = true;  // Successful steal
            break;
          }
        }
      }

      if (!success) {  // No work found in any queue
        if (finished_threads.fetch_add(1) == num_threads - 1) return;  // All threads done
        finished_threads.fetch_sub(1);  // Work might still be available, decrement and continue
        continue;
      }
    }

    // Perform the repeated addition for result[idx]
    float base = result[idx];
    for (int w = 0; w < mult[idx] - 1; w++) {
      result[idx] += base;
    }
  }
}

void launch_threads(float* result_parallel, int* mult) {
  // Initialize queues in parallel
  vector<thread> threads;
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_enq, SIZE, i, NUM_THREADS);
  }

  // Join threads after initialization phase
  for (auto& thread : threads) {
    thread.join();
  }

  threads.clear();  // Reset thread list for parallel_mult

  // Launch the parallel_mult function with workstealing
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  // Join threads after computation phase
  for (auto& thread : threads) {
    thread.join();
  }
}
