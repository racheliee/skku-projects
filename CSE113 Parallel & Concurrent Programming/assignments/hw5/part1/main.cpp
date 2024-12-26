#include <random>
#include <iostream>
#include <chrono>
#if defined(BARRIER)
#include "sense_reversal_barrier.cpp"
#elif defined(SPAWNJOIN)
#include "spawn_join.cpp"
#else
#error "No blur specified."
#endif

#define SIZE 1024 * 32
#define REPEATS (1024 * 8 + 1)
#define FMIN -100.0
#define FMAX 100.0
#define SEED 42

// From https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c/9324796
double fRand()
{
    double f = (double)rand() / RAND_MAX;
    return FMIN + f * (FMAX - FMIN);
}

// By setting the seed of our random number generator, we can get reproducible results.
void set_random_seed() {
    srand(SEED);
}

int main(int argc, char *argv[]) {
  int num_threads = 8;
  if (argc > 1) {
    num_threads = atoi(argv[1]);
  }

  double *input = new double[SIZE];
  double *output = new double[SIZE];

  set_random_seed();
  for (int i = 0; i < SIZE; i++) {
    double randval = fRand();
    input[i] = randval;
    output[i] = randval;    
  } 

  auto time_start = std::chrono::high_resolution_clock::now();
  repeated_blur(input, output, SIZE, REPEATS, num_threads);
  auto time_end = std::chrono::high_resolution_clock::now();
  auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
  double time_seconds = time_duration.count()/1000000000.0;

  std::cout << "time: " << time_seconds << std::endl;
}
