# Queues & Linearizability

## Concurrent Queue
### Definition
- List of items accessed in FIFO
- duplicated allowed
- methods
  - `enq(x)`: add x to the queue
  - `deq()`: remove and return the first item in the queue
  - `size()`: return the number of items in the queue
- three variants
  1. input/output queues
     - multiple threads can enqueue and dequeue but not both
  2. producer/consumer queues
      - one thread enqueues and another dequeues
     1. asynchronous
     2. synchronous

## Input/Output Queue
- queue in which multiple threads can enqueue(write) and dequeue(read) but not both
- computation done in phases:
  - prepare the queue (by writing)
  - all threads join
  - read the queue
- allocate a contiguous array of size $N$ for the queue
  - `front` and `end` pointers

### enqueue
1. reserve a slot
   - 이건 atomic해야하니까 RMW를 사용
2. write the item
3. increment `end`

### dequeue
- 지금 읽고자 하는 index는 atomic하게 읽어야함으로 RMW를 사용
1. read the item
2. increment `front`
3. return the item

## Synchronous Producer/Consumer Queue
- `enq` sets a flag to indicate that an item is available
  - wait for flag to be cleared if the buffer is full
- `deq` waits until the flag is set
  - reset the flag after reading the item

### Implementation
- use a circular buffer
- `head` and `tail` pointers
  - `head` points to the next item to be read
  - `tail` points to the next item to be written
- empty: `head == tail`
- full: `head == (tail + 1) % N`
  - wasting one slot but easier to implement
  - 