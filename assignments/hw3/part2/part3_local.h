#include <thread>
#include <vector>

using namespace std;

ScheduleType t = local_t;

#include "IOQueue.h"
#include <atomic>

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);

void parallel_enq(int size, int tid, int num_threads) {
    // Use this function to enqueue indexes to each of the local
    // workstealing queues
    int chunk = size / num_threads;
    int start = tid * chunk;
    int end = (tid == num_threads - 1) ? size : start + chunk;

    for (int i = start; i < end; ++i) {
        Q[tid].enq(i); // enqueue the index
    }
}

void parallel_mult(float *result, int *mult, int size, int tid, int num_threads) {
    // Implement me using a workstealing approach.
    // You should use the Q IOQueues to compute
    // local work and then try to steal work
    // from others until all the work is completed
    int task;

    // Process local tasks
    while ((task = Q[tid].deq()) != -1) {
        float base = result[task];
        for (int j = 0; j < mult[task] - 1; j++) {
            result[task] += base;
        }
    }

    // Attempt work stealing if local work is done
    while (true) {
        bool has_work = false;

        // Check if all threads are finished
        if (finished_threads.load() >= num_threads) {
            break;
        }

        // Attempt to steal work from other threads’ queues
        for (int i = (tid + 1) % num_threads; i != tid; i = (i + 1) % num_threads) {
            task = Q[i].deq();
            if (task != -1) {
                float base = result[task];
                for (int j = 0; j < mult[task] - 1; j++) {
                    result[task] += base;
                }
                has_work = true;
                break; // Exit loop on successful steal
            }
        }

        if (!has_work) {
            // No work found in any queue, mark thread as finished
            atomic_fetch_add(&finished_threads, 1);
            break;
        }
    }
}

void launch_threads(float *result_parallel, int *mult) {
    // Launch NUM_THREADS threads to intialize the IOQueues in Q with
    // the indexes for each thread. Remember to initialize the Queues
    // with the size that they will need. Join the threads afterwards.

    // Next, launch the parallel function that performs the parallel_mult
    // function from part1_static.h and part2_global.h but using workstealing

    vector<thread> threads;

    // Initialize each IOQueue
    for (int i = 0; i < NUM_THREADS; i++) {
        Q[i].init(SIZE);
    }

    // Enqueue indices in parallel
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(parallel_enq, SIZE, i, NUM_THREADS);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();

    // Reset finished_threads counter for fresh execution
    finished_threads.store(0);

    // Run parallel_mult function with work-stealing strategy
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(parallel_mult, result_parallel, mult, SIZE, i, NUM_THREADS);
    }

    for (auto &thread : threads) {
        thread.join();
    }
}