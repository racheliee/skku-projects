#if defined(SR)
#include "SRBarrier.h"
#elif defined(SRO)
#include "SROBarrier.h"
#else
#error "No barrier specified"
#endif
barrier_object B;

// Do not change the signature of this function.
// However, you should create more functions in this file for your computation.
// At the end, "output" should contain the final blurred values.
void repeated_blur(double* input, double* output, int size, int repeats, int num_threads) {
    // Spawn threads once, use sense reversing barrier
}
