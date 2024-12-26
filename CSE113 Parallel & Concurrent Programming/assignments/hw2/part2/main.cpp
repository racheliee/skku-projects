#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>
using namespace std;
using namespace chrono;

#define SECONDS 1

// Pull in different mutex implementations
#if defined(DEFAULT_MUTEX)
#include "default_mutex.h"

#elif defined(FAIR_MUTEX)

#include "fair_mutex.h"

#else
#error No mutex specified!
#endif

rw_mutex m;

atomic_int total_readers(0);
atomic_int total_writers(0);
atomic_int warmup_total_readers(0);
atomic_int warmup_total_writers(0);

// Function to set CPU affinity for a thread
void set_affinity(thread& t, int core_id) {
  pthread_t native_handle = t.native_handle();

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset);
}

// runs for 1 second, locking and unlocking in the reader state over
// and over
void reader() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    if (warmup_total_readers < 2048) {
      atomic_fetch_add(&warmup_total_readers,1);
    } else {
      atomic_fetch_add(&total_readers,1);
    }
    
    // Perform the lock and unlock
    m.lock_reader();
    m.unlock_reader();
    
    // Update the timer
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }
}

// runs for 1 second, locking and unlocking in the writer (default)
// state over and over
void writer() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    if (warmup_total_writers < 2048) {
      atomic_fetch_add(&warmup_total_writers,1);
    } else {
      atomic_fetch_add(&total_writers,1);
    }
    
    // Perform the lock and unlock
    m.lock();
    m.unlock();
    
    // Update the timer
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }
}

int main(int argc, char *argv[]) {

  // Read number of readers and writers from the command line
  int num_readers = 6;
  if (argc > 1) {
    num_readers = atoi(argv[1]);   
  }
  int num_writers = 2;
  if (argc > 2) {
    num_writers = atoi(argv[2]);
  }

  // Create a new array for the number of threads
  thread *reader_ar = new thread[num_readers];
  thread *writer_ar = new thread[num_writers];

  // Launch the reader threads
  for (int i = 0; i < num_readers; i++) {
    reader_ar[i] = thread(reader);
    // Pin each reader thread to a specific core (e.g., core i)
    set_affinity(reader_ar[i], i);
  }

  // Launch the writer threads
  for (int i = 0; i < num_writers; i++) {
    writer_ar[i] = thread(writer);
    // Pin each reader thread to a specific core (e.g., core i)
    set_affinity(writer_ar[i], num_readers + i);
  }

  // Join the reader threads
  for (int i = 0; i < num_readers; i++) {
    reader_ar[i].join();
  }

  // Join the writer threads
  for (int i = 0; i < num_writers; i++) {
    writer_ar[i].join();
  }

  // Print stats
  cout << "total readers: " << total_readers << endl;
  cout << "total writers: " << total_writers << endl;
}
