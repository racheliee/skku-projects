#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <stdlib.h> 
#include <pthread.h>
using namespace std;
using namespace chrono;

enum ListType { coarse, rw, swaptop };

#if defined(COARSE_LOCK)
#include "coarse_lock_stack.h"
ListType t = coarse;

#elif defined(RW_LOCK)
#include "rw_lock_stack.h"
ListType t = rw;

#elif defined(SWAPTOP)
#include "swaptop_stack.h"
ListType t = swaptop;

#else
#error No stack specified!
#endif
CSE113_Stack S;

#define SECONDS 1

#define PEEKERS 4
#define POPPERS 1
#define PUSHERS 1

atomic_int total_operations(0);
atomic_int total_peeks(0);
atomic_int total_pops(0);
atomic_int total_pushes(0);
atomic_int sync_threads(0);
#if defined(SWAPTOP)
#define SWAPTOPPERS 4
atomic_int total_swaptops(0);
#endif

int my_rand() {
  int choice = rand() % 1000;
  if (choice < 997) {
    return 0;
  }
  return 1;
}

void peekers() {

  atomic_fetch_add(&sync_threads, 1);
  #if defined(SWAPTOP)
  while (sync_threads.load() < SWAPTOPPERS + PEEKERS + POPPERS + PUSHERS);
  #else
  while (sync_threads.load() < PEEKERS + POPPERS + PUSHERS);
  #endif

  auto start = high_resolution_clock::now();
  int duration = 0.0;
  int sleep_counter = 0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_peeks,1);
    
    S.peek();
    
    if (sleep_counter % 100 == 0) {
   	 this_thread::sleep_for(nanoseconds(1));
    }
    sleep_counter++;

    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }
}

void poppers() {
  
  atomic_fetch_add(&sync_threads, 1);
  #if defined(SWAPTOP)
  while (sync_threads.load() < SWAPTOPPERS + PEEKERS + POPPERS + PUSHERS);
  #else
  while (sync_threads.load() < PEEKERS + POPPERS + PUSHERS);
  #endif
  
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  int sleep_counter = 0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_pops,1);
    
    S.pop();
    
    if (sleep_counter % 100 == 0) {
    	this_thread::sleep_for(nanoseconds(1));
    } 
    sleep_counter++;
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}

void pushers() {
  
  atomic_fetch_add(&sync_threads, 1);
  #if defined(SWAPTOP)
  while (sync_threads.load() < SWAPTOPPERS + PEEKERS + POPPERS + PUSHERS);
  #else
  while (sync_threads.load() < PEEKERS + POPPERS + PUSHERS);
  #endif 
  
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  int sleep_counter = 0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_pushes,1);

    int random_value = my_rand();
    S.push(random_value);
    
    if (sleep_counter % 100 == 0) {
    this_thread::sleep_for(nanoseconds(1));
    }
    sleep_counter++;
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}

#if defined(SWAPTOP)
void swaptoppers() {
  
  atomic_fetch_add(&sync_threads, 1);
  while (sync_threads.load() < SWAPTOPPERS + PEEKERS + POPPERS + PUSHERS);

  auto start = high_resolution_clock::now();
  int duration = 0.0;
  int sleep_counter = 0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations, 1);
    atomic_fetch_add(&total_swaptops, 1);

    int random_value = my_rand();
    S.swaptop(random_value);

    if (sleep_counter % 100 == 0) {
    this_thread::sleep_for(nanoseconds(1));
    }
    sleep_counter++;

    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}
#endif

void set_affinity(std::thread &thread, int core_id) {
  pthread_t native_handle = thread.native_handle();
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core_id, &cpuset);

  pthread_setaffinity_np(native_handle, sizeof(cpu_set_t), &cpuset);
}

int main() {

  thread pushers_ar[PUSHERS];
  thread poppers_ar[POPPERS];
  thread peekers_ar[PEEKERS];

  for (int i = 0; i < 1024; i++) {
    int random_value = my_rand();
    S.push(random_value);
  }

#if defined(SWAPTOP)
  thread swaptoppers_ar[SWAPTOPPERS];
#endif
  int cpu_counter = 0;
  for (int i = 0; i < POPPERS; i++) {
    poppers_ar[i] = thread(poppers);
    set_affinity(poppers_ar[i], cpu_counter);
    cpu_counter++;
  }


  for (int i = 0; i < PUSHERS; i++) {
    pushers_ar[i] = thread(pushers);
    set_affinity(pushers_ar[i], cpu_counter);
    cpu_counter++;
  }

  for (int i = 0; i < PEEKERS; i++) {
    peekers_ar[i] = thread(peekers);
    set_affinity(peekers_ar[i], cpu_counter);
    cpu_counter++;
  }

#if defined(SWAPTOP)
  for (int i = 0; i < SWAPTOPPERS; i++) {
    swaptoppers_ar[i] = thread(swaptoppers);
    set_affinity(swaptoppers_ar[i], cpu_counter);
    cpu_counter++;
  }
#endif

  for (int i = 0; i < PUSHERS; i++) {
    pushers_ar[i].join();
  }

  for (int i = 0; i < PEEKERS; i++) {
    peekers_ar[i].join();
  }

  for (int i = 0; i < POPPERS; i++) {
    poppers_ar[i].join();
  }

#if defined(SWAPTOP)
  for (int i = 0; i < SWAPTOPPERS; i++) {
    swaptoppers_ar[i].join();
  }
#endif

  cout << "total operations: " << total_operations << endl;
  cout << "total pops: " << total_pops << endl;

  cout << "total pushes: " << total_pushes << endl;

  cout << "total peeks: " << total_peeks << endl;

 
#if defined(SWAPTOP)
  cout << "total swaptops: " << total_swaptops << endl;
#endif
}

