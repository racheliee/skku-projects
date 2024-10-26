#pragma once

#include <atomic>
#include <mutex>
using namespace std;

class rw_mutex {
 public:
  rw_mutex() {
    writer = false;
    num_readers = 0;
  }

  void lock_reader() {
    bool acquired = false;
    while (!acquired) {
      internal_mutex.lock();
      if (!writer) {
        num_readers++;
        acquired = true;
      }
      internal_mutex.unlock();
    }
  }
  
  void unlock_reader() {
    internal_mutex.lock();
    num_readers--;
    internal_mutex.unlock();
  }

  
  void lock() {
    bool acquired = false;
    while (!acquired) {
      internal_mutex.lock();
      if (!writer && num_readers == 0) {
        writer = true;
        acquired = true;
      }
      internal_mutex.unlock();
    }
  }
  
  void unlock() {
    internal_mutex.lock();
    writer = false;
    internal_mutex.unlock();
  }

 private:
  mutex internal_mutex;
  bool writer;
  int num_readers;
};
