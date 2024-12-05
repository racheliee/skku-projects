#define SRBARRIER // So that the header is not included twice in driver.cpp
#include "SRBarrier.h"

using namespace std;

// This should be the same as part 2.2 but with memory orders changed

void t0_function(atomic_int* x, atomic_int* y, int* r0, barrier_object* B) {
  // complete me!
  B->barrier(0);
  x->store(1, memory_order_seq_cst);
  B->barrier(0);
  *r0 = y->load(memory_order_seq_cst);
}

void t1_function(atomic_int* x, atomic_int* y, int* r1,  barrier_object* B) {
  // complete me!
  B->barrier(1);
  y->store(1, memory_order_seq_cst);
  B->barrier(1);
  *r1 = x->load(memory_order_seq_cst);
}
