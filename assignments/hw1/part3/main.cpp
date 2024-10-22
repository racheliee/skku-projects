#include <iostream>
#include <assert.h>
#include <chrono>
#include "part3.cpp"
using namespace std;
using namespace std::chrono;

/** You may change these constants during development, but your solution will be graded using the initial values. */
#define SIZE 1024
#define K 1048576
#define NUM_THREADS 8

int main() {
  volatile int a[SIZE] = {};
  volatile int b[SIZE] = {};
  volatile int c[SIZE] = {};

  auto seq_start = high_resolution_clock::now();
  // Implement me (in part3.cpp)!
  sequential_increment(a, SIZE, K);
  auto seq_stop = high_resolution_clock::now();
  auto seq_duration = duration_cast<nanoseconds>(seq_stop - seq_start);
  double seq_seconds = seq_duration.count()/1000000000.0;

  auto round_robin_start = high_resolution_clock::now();
  // Implement me!
  round_robin_increment(b, SIZE, K, NUM_THREADS);
  auto round_robin_stop = high_resolution_clock::now();
  auto round_robin_duration = duration_cast<nanoseconds>(round_robin_stop - round_robin_start);
  double round_robin_seconds = round_robin_duration.count()/1000000000.0;

  auto custom_start = high_resolution_clock::now();
  // Implement me!
  custom_increment(c, SIZE, K, NUM_THREADS);
  auto custom_stop = high_resolution_clock::now();
  auto custom_duration = duration_cast<nanoseconds>(custom_stop - custom_start);
  double custom_seconds = custom_duration.count()/1000000000.0;

  double round_robin_speedup = seq_seconds / round_robin_seconds;
  double custom_speedup = seq_seconds / custom_seconds;
  double custom_rr_speedup = round_robin_seconds / custom_seconds;

  std::cout << "\nRound Robin Speedup over Sequential: " << round_robin_speedup << "\n";
  std::cout << "Custom Speedup over Sequential: " << custom_speedup << "\n";
  std::cout << "Custom Speedup over Round Robin: " << custom_rr_speedup << "\n\n";

  return 0;
}


