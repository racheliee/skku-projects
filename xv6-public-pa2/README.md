# Project 2: CPU Scheduling

## Overview
This project involves implementing the Completely Fair Scheduler (CFS) in the xv6 operating system. The CFS scheduler ensures that processes with higher priority receive more CPU time than those with lower priority. You will also modify the `ps` system call to display relevant scheduling information for each process.

## Grade
full score (100/100)

## Objective
- Linux CFS scheduler to ensure runtime increases in accordance with process priority
- `ps` system call to output relevant scheduling information

## Tasks
### 1. Implement CFS on xv6

- **Select process with minimum virtual runtime**: The process with the lowest virtual runtime is scheduled next.
- **Update runtime/vruntime for each timer interrupt**: Adjust these values proportionally based on process weight.
- **Enforce CPU yield**: When a task runs beyond its allocated time slice, it should yield the CPU.
- **Define and use weight for nice values**: Implement weights ranging from 0 to 39, with a default value of 20 (weight of 1024).
- **Time slice calculation**: Use the formula provided for accurate time slice allocation.
- **Handle newly forked and woken processes**:
  - A forked process inherits the parent's runtime, vruntime, and nice value.
  - A woken process’s vruntime is adjusted to maintain fairness.

### 2. Modify the ps System Call

- Output necessary process information:
- runtime/weight
- runtime
- vruntime
- total tick
- Use millitick unit: Represent the runtime and vruntime in milliticks (tick * 1000).
- Avoid floating-point operations: Use integer arithmetic for all calculations.

## `ps` System Call example

```
name    pid    state    priority    runtime/weight   runtime    vruntime    tick
init    1      SLEEPING 5           0                4000       2000        5480000
sh      2      SLEEPING 5           0                1000       0           5480000
mytest  4      RUNNABLE 5           28               832000     38770       5480000
mytest  5      RUNNING  0           37               3370000    38755       5480000
```