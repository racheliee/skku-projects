#include <atomic>

class IOQueue {
 public:
  IOQueue() {
  }


  // Use this function to initialize the queue to
  // the size that you need.
  void init(int size) {
  }

  // enqueue the element e into the queue
  void enq(int e) {
  }

  // return a value from the queue.
  // return -1 if there is no more values
  // from the queue.
  int deq() {
    return -1;
  }

  // return 32 values from the queue. The
  // values should be returned through
  // the array passed in.
  // return 0 if successful. return 1 if not
  // i.e. if the queue does not have 32 elements
  int deq_32(int ret[32]) {
    return -1;
  }

 private:
  // Give me some private variables
};
