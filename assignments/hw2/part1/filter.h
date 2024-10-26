#pragma once

#include <atomic>
#include <thread>
#include <vector>

using namespace std;

class mutex {
  public:
    mutex() {}

    void init(int num_threads) {
        _num_threads = num_threads;
        _level = new atomic<int>[num_threads];
        _victim = new atomic<int>[num_threads];

        // initialize all atomic variables
        for (int i = 0; i < num_threads; ++i) {
            _level[i].store(0);
        }
    }

    void lock(int thread_id) {
        // start from level 1
        for (int i = 1; i < _num_threads; ++i) {
            // indicate entering
            _level[thread_id].store(i);
            _victim[i].store(thread_id);

            // wait until all threads with smaller level or with the same level but with higher priority finish
            for (int j = 0; j < _num_threads; ++j) {
                if (j == thread_id)
                    continue;
                    
                // wait until thread j finishes entering
                while (_level[j].load() >= i && _victim[i].load() == thread_id) {
                    this_thread::yield();
                };
            }
        }
    }

    void unlock(int thread_id) {
        _level[thread_id].store(0); // release lock
    }

  private:
    int _num_threads; // number of threads
    atomic<int> *_level; // level of each thread
    atomic<int> *_victim; // victim of each level
};
