#include <atomic>
#include <vector>
#include <thread>

#if defined(RELAXED)
#include "relaxed_sb.cpp"
#elif defined(SYNC)
#include "sync_sb.cpp"
#elif defined(DISALLOWED)
#include "sc_sb.cpp"
#endif

#ifndef SRBARRIER
#include "SRBarrier.h"
#endif

barrier_object B;

// Fill these out to match your conditions when checking the output
#define OUTPUT_0 "<CONDITION>"
#define OUTPUT_1 "<CONDITION>"
#define OUTPUT_2 "<CONDITION>"
#define OUTPUT_3 "<CONDITION>"

using namespace std;

atomic_int x(0);
atomic_int y(0);

vector<int> test_driver(int iterations) {

  int output0 = 0;
  int output1 = 0;
  int output2 = 0;
  int output3 = 0;

  for (int i = 0; i < iterations; i++) {
    // Run a test iteration

    // Record a histogram, fill in the conditions
    if (false) {
      output0++;
    }
    else if (false) {
      output1++;
    }
    else if (false) {
      output2++;
    }
    // This should be the relaxed behavior
    else if (false) {
      output3++;
    }
  }

  vector<int> res {output0, output1, output2, output3};
  return res;
}
