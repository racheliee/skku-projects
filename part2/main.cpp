#include <thread>
#include "util.h"
#include <chrono>
#include <iostream>
#include "checker.h"

#if defined(PART1)
#include "part1_static.h"
#elif defined(PART2)
#include "part2_global.h"
#elif defined(PART3)
#include "part3_local.h"
#elif defined(PART4)
#include "part4_local32.h"
#endif

int main() {
  float* result_parallel = new float[SIZE];
  int* mult = new int[SIZE];
  for (int i = 0; i < SIZE; i++) {
    result_parallel[i] = i;
  }
  linear_work(mult, SIZE);

  auto start = std::chrono::high_resolution_clock::now();

  // Implemented by the header for each part
  launch_threads(result_parallel, mult);

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  double seconds = duration.count()/1000000000.0;
  check_results(result_parallel, SIZE, t);
  check_timing(seconds, t);

  std::cout << "time (seconds): " << seconds << std::endl;

  return 0;
}
