# Concurrent Data Structures

## Data Conflicts/Races Catastrophes
> There are no benign data races

- Therac 25: radiation therapy gone wrong
- 2003 NE power blackout

## Tools to find data conflicts
- hapens before
  - build partial order of mutex locks and unlocks
  - any memory accesses that can't be ordered in this partial order is a conflict
  - transitive closure of the critical sections
- lockset
  - every shared memeory location is associated with a set of locks
  - refine lockset on every access
  - if location is later access without the previous locks for the location --> report conflict
- thread sanitizer
  - dynamic analysis tool passed to the compiler
  - 10x overhead 
  - identifies data conflicts & deadlocks
- Meta Infer    
  - statiscally chesk for amny issues (works on source code)
  - check for races in concurrent classes

## Concurrent Data Structures
- abstraction, encapsulation, and modularity/composability
- no need to use locks to access/mofify the data structure

### examples
- `fflush`를 사용해서 buffer를 비워주는 것도 가능 (slow)
- threads can access the same data structure concurrently
- quadtree
  - 각 네모의 computation 양이 비슷하도록 그림을 쪼갬
  - this also uses a concurrent data structure

### Properties
- correctness
  - how they should behave (specification)
- performance
- fairness
  - under what conditions can they deadlock

## Concurrent queue
- sequential specification
  - `enqueue` and `dequeue` operations

### sequential consistency
- valid executions correspond a sequentialization of object method calls
- 함수가 불려지는 순서들은 바껴도, 함수 안에 있는 instruction들의 순서들이 바뀌진 않음
  - instructions inside methods cannot be interleaved
- not enough specification for concurrent data structures
  - 실제 함수의 순서에 대한 제약을 제공하지 않음
  - 