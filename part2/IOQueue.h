#include <atomic>
#include <vector>

using namespace std;

class IOQueue {
 public:
  IOQueue(): head(0), tail(0), capacity(0){}

  // Use this function to initialize the queue to
  // the size that you need.
  void init(int size) {
    buffer.resize(size);
    capacity = size;
    head.store(0);
    tail.store(0);
  }

  // enqueue the element e into the queue
  void enq(int e) {
    int t = tail.load(); 
    int h = head.load();
    if((t+1)%capacity == h){
      return;
    }
    buffer[t] = e;
    tail.store((t+1)%capacity);
  }

  // return a value from the queue.
  // return -1 if there is no more values
  // from the queue.
  int deq() {
    int h = head.load();
    if(h == tail.load()){
      return -1;
    }

    int res = buffer[h];
    head.store((h+1)%capacity);
    return res;
  }

  // return 32 values from the queue. The
  // values should be returned through
  // the array passed in.
  // return 0 if successful. return 1 if not
  // i.e. if the queue does not have 32 elements
  int deq_32(int ret[32]) {
    int h = head.load();
    int t = tail.load();
    if ((t - h + capacity) % capacity < 32) {
      // Not enough elements to dequeue 32
      return 1;
    }
    for (int i = 0; i < 32; i++) {
      ret[i] = buffer[(h + i) % capacity];
    }
    head.store((h + 32) % capacity);
    return 0;
  }

 private:
  // Give me some private variables
  atomic<int> head;
  atomic<int> tail;
  int capacity;
  vector<int> buffer;
};
