#if defined(SR)
#include "SRBarrier.h"
#elif defined(SRO)
#include "SROBarrier.h"
#else
#error "No barrier specified"
#endif

#include <thread>
#include <vector>

using namespace std;

barrier_object B;

void blur_chunk(double *input, double *output, int start, int end, int repeats, int tid) {
    for (int r = 0; r < repeats; ++r) {
        // Perform blur operation for the assigned chunk
        for (int i = start; i < end; ++i) {
            output[i] = (input[i - 1] + input[i] + input[i + 1]) / 3.0;
        }

        B.barrier(tid);  // Synchronize all threads after computation

        if (r < repeats - 1) {
            swap(input, output);  // Swap input and output for the next iteration
        }

        B.barrier(tid);  // Synchronize all threads before the next iteration
    }
}

// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
void repeated_blur(double *input, double *output, int size, int repeats, int num_threads) {
    // Spawn threads once, use sense reversing barrier

    B.init(num_threads);

    int chunk_size = (size -2 + num_threads -1) / num_threads;
    vector<thread> threads;

    for(int t = 0; t < num_threads; ++t){
        int start = t * chunk_size + 1;
        int end = min(size - 1, start + chunk_size);
        threads.emplace_back(blur_chunk, input, output, start, end, repeats, t);
    }

    for(auto &t : threads)
        t.join();
    
    if(repeats % 2 == 1) {
        for(int i = 0; i < size; ++i)
            output[i] = input[i];
    }
}
