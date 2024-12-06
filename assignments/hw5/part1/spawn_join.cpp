#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

// Function for each thread to perform blur on its chunk of the array
void blur_chunk(double* input, double* output, int start, int end) {
    for (int i = start; i < end; ++i) {
        output[i] = (input[i - 1] + input[i] + input[i + 1]) / 3.0;
    }
}

// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    // Spawn and join threads repeatedly.
    int chunk_size = (size - 2) / num_threads; // Divide the array into chunks excluding boundaries

    for (int r = 0; r < repeats; ++r) {
        std::vector<std::thread> threads;

        for (int t = 0; t < num_threads; ++t) {
            int start = 1 + t * chunk_size; // Start index for this thread
            int end = (t == num_threads - 1) ? (size - 1) : (start + chunk_size); // End index for this thread

            threads.emplace_back(blur_chunk, input, output, start, end);
        }

        // Join all threads
        for (auto& th : threads) {
            th.join();
        }

        // Swap input and output arrays
        std::swap(input, output);
    }

    // If the number of repeats is odd, swap back the results to the output array
    if (repeats % 2 == 1) {
        for (int i = 0; i < size; ++i) {
            output[i] = input[i];
        }
    }
}
