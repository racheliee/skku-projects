#pragma once

#include <atomic>
#include <mutex>
using namespace std;

class rw_mutex {
  public:
    rw_mutex() {
        _num_readers = 0;
        _writer = false;
        _writer_waiting.store(false);
    }

    void lock_reader() {
        bool acquired = false;
        while (!acquired) {
            m.lock();
            if (!_writer && !_writer_waiting) { // no writer and no writer waiting
                _num_readers++;
                acquired = true;
            }
            m.unlock();
        }
    }

    void unlock_reader() {
        m.lock();
        _num_readers--;
        m.unlock();
    }

    void lock() {
        _writer_waiting.store(true); // indicate that writer is waiting
        bool acquired = false;
        while (!acquired) {
            m.lock();
            if (!_writer && !_num_readers) {
                _writer = true;
                acquired = true;
            } else {
                // indicate that writer is still waiting
                _writer_waiting.store(true); 
            }
            m.unlock();
        }
    }

    void unlock() {
        m.lock();
        _writer = false;
        _writer_waiting = false; // reset writer waiting
        m.unlock();
    }

  private:
    mutex m;
    int _num_readers;
    bool _writer;
    atomic<bool> _writer_waiting; // indicate if writer is waiting
};
