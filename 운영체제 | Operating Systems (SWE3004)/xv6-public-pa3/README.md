# OS Project 3: Virtual Memory

## Overview
This assignment involves implementing virtual memory support in the xv6 operating system. Implement the `mmap()`, `munmap()`, and `freemem()` system calls, as well as a page fault handler to handle page faults for memory accesses to mapped regions.

## Grade
60/100 >:(

## Objective
- Implementing `mmap()` system call
- Implementing `munmap()` system call
- Implementing `freemem()` system call
- Implementing a page fault handler

## Tasks
### 1. Implement `mmap()` System Call

- **Function Prototype**: 
  ```c
  uint mmap(uint addr, int length, int prot, int flags, int fd, int offset);
  ```
- **Parameters**:
  - `addr`: Page-aligned start address for mapping
  - `length`: Length of the mapping, multiple of page size
  - `prot`: Protection flags (PROT_READ, PROT_WRITE)
  - `flags`: Mapping flags (MAP_ANONYMOUS, MAP_POPULATE)
  - `fd`: File descriptor for file mappings
  - `offset`: Offset within the file for the mapping
- **Behavior**:
  - Returns the start address of the mapping area on success, 0 on failure
  - Handles both anonymous and file mappings
  - Populates physical pages and constructs page tables if MAP_POPULATE is specified

### 2. Implement `munmap()` System Call

- **Function Prototype**: 
  ```c
  int munmap(uint addr);
  ```
- **Parameters**:
  - `addr`: Page-aligned start address of the mapping to unmap
- **Behavior**:
  - Returns 1 on success, -1 on failure
  - Removes the corresponding `mmap_area` structure
  - Frees physical pages and page tables if allocated, otherwise just removes the `mmap_area`

#### 3. Implement `freemem()` System Call

- **Function Prototype**: 
  ```c
  int freemem(void);
  ```
- **Behavior**:
  - Returns the current number of free memory pages
  - Updates the free memory count when pages are allocated or freed

### 4. Implement Page Fault Handler

- **Behavior**:
  - Handles page faults for accesses to mapped regions without allocated physical pages or page tables
  - Allocates physical pages and constructs page tables on demand
  - Terminates the process on failure

## Implementation Details

- **mmap**:
  - Manages memory mappings using `mmap_area` structures
  - Maintains an array of `mmap_area` structures with a maximum of 64 entries

- **Page Fault Handling**:
  - Catch page faults using interrupt 14 (T_PGFLT)
  - Use `rcr2()` to determine the faulting address
  - Allocate physical pages and construct page tables as needed

## Sample `mmap` Usage

- **File Mapping with MAP_POPULATE**:
  ```c
  mmap(0, 8192, PROT_READ, MAP_POPULATE, fd, 4096);
  ```
- **Anonymous Mapping with MAP_POPULATE**:
  ```c
  mmap(0, 8192, PROT_READ | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
  ```