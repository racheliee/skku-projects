#include <atomic>
#include <vector>

using namespace std;

class CQueue {
 public:

  CQueue() : front(0), back(0), count(0) {
    capacity = 1024;
    buffer.resize(capacity);
  }

  void enq(float e) {
    // Implement me!
    while(count.load() == capacity);

    buffer[back] = e;
    back = (back + 1) % capacity;

    count.fetch_add(1);
  }

  float deq() {
    // Implement me!
    while(count.load() == 0);

    float res = buffer[front];
    front = (front + 1) % capacity;

    count.fetch_sub(1);

    return res;
  }
  
 private:
  // Give me some private variables 
  vector<float> buffer;
  size_t front, back, capacity;
  atomic<size_t> count; // keep track of the number of elements in the queue
};
