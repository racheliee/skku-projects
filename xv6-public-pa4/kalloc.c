// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "fs.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
} kmem;

// swap space structure
struct {
  struct spinlock lock;
  int use_lock;
  char* bitmap; // char* since kalloc returns char*, index is the block number (blkno)
} swap_space;

struct page pages[PHYSTOP/PGSIZE];
struct page *page_lru_head;
int num_free_pages;
int num_lru_pages;

struct spinlock lru_lock;
// total number of swap pages = SWAPMAX / 8
const int last_bit_map_index = (SWAPMAX) / 8 / (PGSIZE / BSIZE); 


// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  // initialize variables
  num_free_pages = 0;
  for(int i = 0; i < PHYSTOP/PGSIZE; i++){
    pages[i].pgdir = 0;
    pages[i].vaddr = 0;
    pages[i].next = 0;
    pages[i].prev = 0;
  }
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;

  // initialize LRU list
  num_lru_pages = 0;
  page_lru_head = 0;

  // initialize swap space
  swap_space.bitmap = kalloc();
  if(swap_space.bitmap == 0)
    panic("kinit1: swap_space.bitmap is null");

  // set swap_space bitmaps
  // first 8 bits are reserved for the bitmap itself (since 8 blocks are used for 1 page)
  acquire(&swap_space.lock);
  memset((void*)swap_space.bitmap, 0xFF, PGSIZE);
  
  // set the rest of the swap_space bitmap to 0's
  memset((void*)(swap_space.bitmap + PGSIZE), 0, last_bit_map_index + 1 - PGSIZE);
  
  // terminate the bitmap
  swap_space.bitmap[last_bit_map_index] |= 1 << (SWAPMAX % 8);
  release(&swap_space.lock);
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  // Increment the number of free pages
  num_free_pages++;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

try_again:
  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
 if(!r && reclaim())
	  goto try_again;
  if(r)
    kmem.freelist = r->next;
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

// if free page is not obtained through kalloc, swap-out the victim page
int
reclaim(void)
{


}

void 
add_page_to_lru(struct page* p)
{

}

void
delete_page_from_lru(struct page* p)
{

}

