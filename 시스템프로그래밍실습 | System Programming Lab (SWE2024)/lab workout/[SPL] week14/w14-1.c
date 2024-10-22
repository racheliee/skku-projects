#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_data {
  struct random_data* random_state;
  size_t points;
} thread_data;

void* do_monte_carlo(void* arg) {
  int x_int, y_int;
  double x, y;
  thread_data* data = (thread_data*)arg;

  /*
    TODO: Write code to implement Monte Carlo here
    You can modify the code here however you want
  */

  for (size_t i = 0; i < data->points; i++) {
    random_r(data->random_state, &x_int);
    random_r(data->random_state, &y_int);
    x = (double)x_int / RAND_MAX;
    y = (double)y_int / RAND_MAX;

    // TODO: check if it is in the quadrant
  }

  // Do other stuff here if needed

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("usage: %s <num_threads> <points per thread>\n", argv[0]);
    exit(1);
  }
  long n = strtol(argv[1], NULL, 10);
  size_t points = strtol(argv[2], NULL, 10);

  srandom(time(NULL));
  pthread_t threads[n];
  thread_data data[n];

  struct random_data random_states[n];
  memset(random_states, 0, sizeof(random_states));
  char random_statebufs[n][32];
  memset(random_statebufs, 0, sizeof(random_statebufs));

  for (int i = 0; i < n; i++) {
    initstate_r(random(), random_statebufs[i], sizeof(random_statebufs[i]),
                &random_states[i]);
    data[i].random_state = &random_states[i];
    data[i].points = points;

    // TODO: Create thread
  }

  // TODO: Join threads

  /* cannot change lines below */
  double pi = (double)(4 * count) / (double)(n * points);
  printf("pi: %lf\n", pi);
  return 0;
}