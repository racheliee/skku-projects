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

        for (int i = 0; i < num_threads; ++i) {
            _entering[i].store(0);
            _number[i].store(0);
        }
    }

    void lock(int thread_id) {
        _entering[thread_id].store(1, memory_order_relaxed);

        int max_ticket = 0;
        for (int i = 0; i < _num_threads; ++i) {
            int t = _number[i].load(memory_order_relaxed);
            if (t > max_ticket)
                max_ticket = t;
        }

        _number[thread_id].store(1 + max_ticket, memory_order_relaxed);

        _entering[thread_id].store(0, memory_order_relaxed);

        for (int j = 0; j < _num_threads; ++j) {
            if (j == thread_id)
                continue;

            while (_entering[j].load(memory_order_relaxed)) {
                this_thread::yield();
            };

            while (_number[j].load(memory_order_relaxed) != 0 &&
                   (_number[j].load(memory_order_relaxed) < _number[thread_id].load(memory_order_relaxed) ||
                    (_number[j].load(memory_order_relaxed) == _number[thread_id].load(memory_order_relaxed) && j < thread_id))) {
                this_thread::yield();
            };
        }
    }

    void unlock(int thread_id) {
        _number[thread_id].store(0, memory_order_relaxed);
    }

  private:
    int _num_threads;
    atomic<int> *_entering;
    atomic<int> *_number;
};
