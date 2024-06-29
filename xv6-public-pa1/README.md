# Project 1: System Call
## Overview
This project involves creating three new system calls in the xv6 operating system: `getnice`, `setnice`, and `ps`. These system calls manage and retrieve the "nice" value of processes, influencing their scheduling priority.  

## Grade
Full Score: 100/100

## Objective
System calls:
  - `getnice(int pid)`: Retrieves the nice value of a process.
  - `setnice(int pid, int value)`: Sets the nice value of a process.
  - `ps(int pid)`: Prints process information, including name, pid, state, and nice value.