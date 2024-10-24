#define SIZE (1024*1024)
#define NUM_THREADS (8)

void linear_work(int* work, int size) {
  for (int i = 0; i < size; i++) {
    int new_i = (i / 128) + 1;
    work[i] = new_i;
  }
}
