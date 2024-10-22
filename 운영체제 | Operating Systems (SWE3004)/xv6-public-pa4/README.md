# OS Project 4: Page Replacement

## Overview
This assignment involves implementing page-level swapping and managing swappable pages with the LRU list. You will implement the swap-in and swap-out operations, manage swappable pages with the LRU list, and implement the clock algorithm for page replacement.

## Grade
80/100

## Objective

- Implement page-level swapping
  - Swap-in: move the victim page from backing store to main memory
  - Swap-out: move the victim page from main memory to backing store
- Manage swappable pages with LRU list
  - Page replacement policy: clock algorithm

## Tasks

### 1. Implement Swap-in and Swap-out Operations

- **Swap-out Operation**:
  - Write the victim page to swap space using `swapwrite()` function.
  - Set the victim page’s PTE to swap space offset.
  - Clear the PTE_P bit.
- **Swap-in Operation**:
  - Allocate a new physical page.
  - Read from swap space into the physical page using `swapread()` function.
  - Update the PTE value with the physical address and set the PTE_P bit.

### 2. Manage Swappable Pages with LRU List

- **LRU List Management**:
  - Use a circular doubly linked list to manage swappable pages.
  - Implement the clock algorithm using the A (Accessed) bit in each PTE (PTE_A : 0x20).
  - When `kalloc()` fails to allocate a free page, initiate page reclamation by swapping out a victim page.

### 3. Page Replacement Algorithm: Clock Algorithm

- **Algorithm Details**:
  - Use the A bit to track page accesses.
  - Clear the A bit and move the page to the tail of the LRU list if PTE_A == 1.
  - Select the page for eviction if PTE_A == 0.
  - Ensure synchronization with appropriate locks when managing shared resources.


> **Swappable Pages**:
> 
>  - Only user pages are swappable, excluding pages like page table pages.
>  - Manage all pages in a `struct page`, which includes fields for LRU list management and PTEs.

## Sample Code and Functions

### Skeleton Code Functions
  - `void swapread(char* ptr, int blkno);`
  - `void swapwrite(char* ptr, int blkno);`
  - `void swapstat(int* nr_sectors_read, int* nr_sectors_write);`

### Page Structure

  ```c
  struct page {
      struct page *next; // for LRU list
      struct page *prev; // for LRU list
      pde_t *pgdir;
      char *vaddr;
  };
  struct page pages[PHYSTOP/PGSIZE];
  ```