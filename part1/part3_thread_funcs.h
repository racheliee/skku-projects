#include <math.h>

void memory_load_thread_func(float *a, int size) {
  for (int i = 0; i < size; i += 8) {
    memory_to_trig.enq_8(a + i);  // Enqueue 8 elements from `a` to `memory_to_trig`
  }
}

void trig_thread_func(int size) {
  float batch[8];
  for (int i = 0; i < size; i += 8) {
    memory_to_trig.deq_8(batch);           // Dequeue 8 elements from `memory_to_trig`
    for (int j = 0; j < 8; ++j) {
      batch[j] = cos(batch[j]);            // Compute cosine for each element
    }
    trig_to_memory.enq_8(batch);           // Enqueue the result to `trig_to_memory`
  }
}

void memory_store_thread_func(float *a, int size) {
  for (int i = 0; i < size; i += 8) {
    trig_to_memory.deq_8(a + i);           // Dequeue 8 elements from `trig_to_memory` to `a`
  }
}
