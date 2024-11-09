#include <thread>
#include <vector>

using namespace std;

ScheduleType t = local_t;

#include <atomic>
#include "IOQueue.h"

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
  // Use this function to enqueue indexes to each of the local
  // workstealing queues
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
  int task;
  while ((task = Q[tid].deq()) != -1) {  // Process local tasks
      float base = result[task];
      for (int j = 0; j < mult[task] - 1; j++) {
          result[task] += base;
      }
  }

  atomic_fetch_add(&finished_threads, 1);

  while (finished_threads.load() < num_threads) {  // Continue to steal until all threads finish
      for (int i = 0; i < num_threads; i++) {
          if (i != tid) {  // Try to steal from other threads
              task = Q[i].deq();
              if (task != -1) {
                  float base = result[task];
                  for (int j = 0; j < mult[task] - 1; j++) {
                      result[task] += base;
                  }
              }
          }
      }
  }
}

void launch_threads(float* result_parallel, int* mult) {
  // Launch NUM_THREADS threads to intialize the IOQueues in Q with
  // the indexes for each thread. Remember to initialize the Queues
  // with the size that they will need. Join the threads afterwards.

  // Next, launch the parallel function that performs the parallel_mult
  // function from part1_static.h and part2_global.h but using workstealing

  vector<thread> threads;

  for(int i = 0; i < NUM_THREADS; i++) {
    Q[i].init(SIZE);
  }

  // initial queues in parallel
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_enq, SIZE, i, NUM_THREADS);
  }

  for(auto& thread : threads) {
    thread.join();
  }

  threads.clear();

  // Launch the parallel_mult function with workstealing
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
  }

  for(auto& thread : threads) {
    thread.join();
  }

}