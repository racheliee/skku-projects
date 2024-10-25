#pragma once

#include <atomic>
#include <memory>
#include <vector>
#include <thread>

using namespace std;

class mutex {
  public:
    mutex() {}

    void init(int num_threads) {
        _num_threads = num_threads;
        _level.resize(num_threads);
        _victim.resize(num_threads);

        for (int i = 0; i < num_threads; ++i) {
            _level[i] = make_unique<atomic<int>>(0);
            _victim[i] = make_unique<atomic<int>>(0);
        }
    }

    void lock(int thread_id) {
        for (int i = 1; i < _num_threads; ++i) {
            _level[thread_id]->store(i);
            _victim[i]->store(thread_id);

            for (int j = 0; j < _num_threads; ++j) {
                if (j == thread_id)
                    continue;
                while (_level[j]->load() >= i && _victim[i]->load() == thread_id) {
                  this_thread::yield();
                };
            }
        }
    }

    void unlock(int thread_id) {
        _level[thread_id]->store(0);
    }

  private:
    int _num_threads;
    vector<unique_ptr<atomic<int>>> _level;
    vector<unique_ptr<atomic<int>>> _victim;
};
