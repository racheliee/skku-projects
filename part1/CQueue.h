#include <atomic>
#include <array>

using namespace std;

#define CQUEUE_SIZE 2048

class CQueue {
 public:

  CQueue(): front(0), back(0) {
    // Implement me!
    count.store(0);
    buffer.fill(0.0);
  }

  void enq(float e) {
    // Implement me!
    while(count.load() == CQUEUE_SIZE);

    buffer[back] = e;
    back = (back + 1) % CQUEUE_SIZE;
    count.fetch_add(1);
  }

  float deq() {
    // Implement me!
    while(count.load() == 0);

    float res = buffer[front];
    front = (front + 1) % CQUEUE_SIZE;
    count.fetch_add(-1);

    return res;
  }

  int size() {
    // Implement me!
    return count.load();
  }

  void deq_8(float e[8]) {
    // Implement me for part 3
    for(int i = 0; i < 8; ++i){
      e[i] = deq();
    }
  }

  void enq_8(float e[8]) {
    // Implement me for part 3
    for(int i = 0; i < 8; ++i){
      enq(e[i]);
    }
  }
  
 private:
  // give me some variables!
  array<float, CQUEUE_SIZE> buffer;
  size_t front, back;
  atomic<size_t> count;
};
