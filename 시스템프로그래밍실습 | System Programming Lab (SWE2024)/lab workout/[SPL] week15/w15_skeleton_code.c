#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define QSIZE 5
#define LOOP 30

typedef struct {
  int put_index;  // rear
  int get_index;  // front
  int length;     // size
  int capacity;
  int n_loops;
  pthread_mutex_t lock;
  pthread_cond_t not_full;
  pthread_cond_t not_empty;
  int data[];
} queue_t;

void* produce(void* args);
void* consume(void* args);
void put_data(queue_t* queue, int data);
int get_data(queue_t* queue);

queue_t* qinit(int capacity, int n_loops) {
  queue_t* queue = (queue_t*)malloc(sizeof(queue_t) + sizeof(int[capacity]));
  queue->capacity = capacity;
  queue->n_loops = n_loops;
  queue->put_index = queue->get_index = queue->length = 0;
  pthread_mutex_init(&queue->lock, NULL);
  pthread_cond_init(&queue->not_full, NULL);
  pthread_cond_init(&queue->not_empty, NULL);
  return queue;
}

void qdelete(queue_t* queue) {
  pthread_mutex_destroy(&queue->lock);
  pthread_cond_destroy(&queue->not_full);
  pthread_cond_destroy(&queue->not_empty);
  free(queue);
}

int main() {
  queue_t* queue = qinit(QSIZE, LOOP);

  pthread_t producer, consumer;
  pthread_create(&producer, NULL, produce, (void*)queue);
  pthread_create(&consumer, NULL, consume, (void*)queue);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);

  qdelete(queue);
}

void* produce(void* args) {
  int i, data;
  queue_t* queue = (queue_t*)args;
  for (i = 0; i < queue->n_loops; i++) {
    data = i;
    put_data(queue, data);
  }
  pthread_exit(NULL);
}

void* consume(void* args) {
  int i;
  queue_t* queue = (queue_t*)args;
  for (i = 0; i < queue->n_loops; i++) {
    get_data(queue);
  }
  pthread_exit(NULL);
}

/** Add your code here **/

// Put data unless queue is full
void put_data(queue_t* queue, int data) {
  // 1. Lock
  // 2. Wait until queue is not full
  // 3. Put Algorithm for Circular Array
  // 3.a. Assign the value `data` to the position pointed by `put_index`
  // 3.b. Modularly increase `put_index` by 1 so that it is within `capacity` at
  // all times
  // 3.c. Increase `length` by 1

  printf("put data %d to queue\n", data);

  // 4. Send a signal that queue is not empty
  // 5. Unlock
}

// Get data unless queue is empty
int get_data(queue_t* queue) {
  int data;
  // 1. Lock
  // 2: Wait until queue is not empty
  // 3. Get Algorithm for Circular Array
  // 3.a. Get the data at index `get_index` and then modularly add `get_index`
  // by
  //      so that it is within capacity at all times
  // 3.b. Decrease `length` by 1
  // 3.c. When `length` is zero, set `get_index` and `put_index` to 0

  printf("get data %d from queue\n", data);

  // 4. Send a signal that queue is not full
  // 5. Unlock
  // Note: Signal before releasing the lock because the signal thread might
  // acquire the lock again.
  // 6. Return data
  return data;
}

/** End **/