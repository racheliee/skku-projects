#pragma once

#include <algorithm>
#include <atomic>
#include <thread>
#include <vector>

using namespace std;

class mutex {
  public:
    mutex() {}

    void init(int num_threads) {
        _num_threads = num_threads;
        _entering = new atomic<int>[num_threads];
        _number = new atomic<int>[num_threads];

        // initialize all atomic variables
        for (int i = 0; i < num_threads; ++i) {
            _entering[i].store(0);
            _number[i].store(0);
        }
    }

    void lock(int thread_id) {
        _entering[thread_id].store(1); // indicate entering

        // find the maximum ticket number
        int max_ticket = 0;
        for (int i = 0; i < _num_threads; ++i) {
            int t = _number[i].load();
            if (t > max_ticket)
                max_ticket = t;
        }

        _number[thread_id].store(1 + max_ticket);

        _entering[thread_id].store(0); // indicate done entering

        for (int j = 0; j < _num_threads; ++j) {
            if (j == thread_id)
                continue;
            
            // wait until thread j finishes entering
            while (_entering[j].load()) {
                this_thread::yield();
            };

            // wait until all threads with smaller number or with the same number but with higher priority finish
            while (_number[j].load() != 0 &&
                   (_number[j].load() < _number[thread_id].load() ||
                    (_number[j].load() == _number[thread_id].load() && j < thread_id))) {
                this_thread::yield();
            };
        }
    }

    void unlock(int thread_id) {
        _number[thread_id].store(0); // release lock
    }

  private:
    int _num_threads; // number of threads
    atomic<int> *_entering; // whether thread is entering
    atomic<int> *_number; // ticket number
};
