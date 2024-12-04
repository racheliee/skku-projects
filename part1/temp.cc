// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
#include <thread>
#include <vector>
#include <iostream>

void blur(double* input, double* output, int chunk_size, int thread_id, int num_threads){
    int start = thread_id * chunk_size + 1;//Don't calculate for the first double
    int end = thread_id == num_threads - 1 ? 
        (thread_id + 1) * chunk_size - 1 : // Last thread, don't calculate blur for last blur
        (thread_id + 1) * chunk_size + 1; // base case

    for (int i = start; i < end; i++){
        output[i] = (input[i - 1] + input[i] + input[i + 1]) / 3;
    }
}

void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    int chunk_size = size / num_threads;

    for (int r = 0; r < repeats; r++){
        std::vector<std::thread> threads; //Reset vector with threads.

        for (int t_id = 0; t_id < num_threads; t_id++){
            threads.push_back(std::thread(blur, input, output, chunk_size, t_id, num_threads));
        }


        for (int t_id = 0; t_id < num_threads; t_id++){
            threads[t_id].join();
        }

        //write_in_out(input, output, size);

        double *tmp = input;
        input = output;
        output = tmp;
    }


}