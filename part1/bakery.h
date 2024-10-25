#pragma once

#include <algorithm>
#include <atomic>
#include <memory>
#include <thread>
#include <vector>

using namespace std;

class mutex {
  public:
    mutex() {}

    void init(int num_threads) {
        _num_threads = num_threads;
        _entering.resize(num_threads);
        _number.resize(num_threads);

        for (int i = 0; i < num_threads; ++i) {
            _entering[i] = make_unique<atomic<int>>(0);
            _number[i] = make_unique<atomic<int>>(0);
        }
    }

    void lock(int thread_id) {
        _entering[thread_id]->store(1);

        int max_ticket = 0;
        for (int i = 0; i < _num_threads; ++i) {
            int t = _number[i]->load();
            if (t > max_ticket)
                max_ticket = t;
        }

        _number[thread_id]->store(1 + max_ticket);

        _entering[thread_id]->store(0);

        for (int j = 0; j < _num_threads; ++j) {
            if (j == thread_id)
                continue;

            while (_entering[j]->load()) {
                this_thread::yield();
            };

            while (_number[j]->load() != 0 &&
                  (_number[j]->load() < _number[thread_id]->load() ||
                  (_number[j]->load() == _number[thread_id]->load() && j < thread_id))) {
                this_thread::yield();
            };
        }
    }

    void unlock(int thread_id) {
        _number[thread_id]->store(0);
    }

  private:
    int _num_threads;
    vector<unique_ptr<atomic<int>>> _entering;
    vector<unique_ptr<atomic<int>>> _number;
};
