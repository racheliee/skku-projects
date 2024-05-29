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

// total number of swap pages = SWAPMAX / 8
#define LAST_BITMAP_INDEX (SWAPMAX / 8 / (PGSIZE / BSIZE)) // last bitmap index of the bitmap array
#define MAX_BITMAP_SIZE (SWAPMAX / (PGSIZE / BSIZE)) // maximum number of blocks that can be used for swap space

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
  char* bitmap; // char* since kalloc returns char*, index is the block number (blkno)
  int num_free_blocks; // number of free blocks in the swap space
} swap_space;

struct page pages[PHYSTOP/PGSIZE];
struct page *page_lru_head;
int num_free_pages;
int num_lru_pages;
struct spinlock page_lock;

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
  initlock(&page_lock, "page_lock");
  num_lru_pages = 0;
  page_lru_head = 0;

  // initialize swap space
  initlock(&swap_space.lock, "swap_space");
  swap_space.bitmap = kalloc();
  if(swap_space.bitmap == 0)
    panic("kinit1: swap_space.bitmap is null");

  // set swap_space bitmaps
  // first 8 bits are reserved for the bitmap itself (since 8 blocks are used for 1 page)
  acquire(&swap_space.lock);
  swap_space.num_free_blocks = MAX_BITMAP_SIZE;

  memset((void*)swap_space.bitmap, 0, PGSIZE); // should the first 8 bits be 0 or 1???
  
  // set the rest of the swap_space bitmap to 0's
  // memset((void*)(swap_space.bitmap + PGSIZE), 0, LAST_BITMAP_INDEX + 1 - PGSIZE);
  // memset((void*)(swap_space.bitmap), 0, LAST_BITMAP_INDEX + 1);
  
  // terminate the bitmap
  swap_space.bitmap[LAST_BITMAP_INDEX] |= 1 << (SWAPMAX % 8);
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
  if(!r){
    // if kalloc is not successful, try to reclaim a page
    if(reclaim())
      goto try_again;
    // if reclaim fails, OOM (out of memory); return 0;
    else{
      cprintf("kalloc: out of memory\n");
      return 0; // kmem.lock is already released in reclaim()
    }
  }
  if(r)
    kmem.freelist = r->next;
  if(kmem.use_lock)
    release(&kmem.lock);
  
  // Decrement the number of free pages if kalloc is successful
  num_free_pages--;
  return (char*)r;
}

// swap-out logic
// if free page is not obtained through kalloc, swap-out the victim page
int
reclaim(void)
{ 
  // if there are no pages in the LRU list, return 0
  if(page_lru_head == 0 || num_lru_pages == 0){ //num_lru_pages == 0
    // cprintf("reclaim: no pages in the LRU list\n");
    return 0;
  }
  
  // release the lock to avoid deadlock
  // also, if released in kalloc, panic "release" occurs loll idk why lmao
  release(&kmem.lock); 

  // finding the victim page
  acquire(&page_lock); // acquire the page lock

  struct page *victim = page_lru_head;
  pte_t *pte = 0;

  // iterate through the LRU list until a pge with PTE_A == 0 is found
  while(1){
    pte = walkpgdir(victim->pgdir, (void*)victim->vaddr, 0);

    // if PTE_A == 1, clear it and send it to the tail of the LRU list
    if(*pte & PTE_A){
      // reset the victim page's PTE_A bit
      *pte &= ~PTE_A;

      // move victim to tail by moving the head to the next page
      page_lru_head = victim->next;

      // move to the next page
      victim = victim->next;
    } 
    // if PTE_A == 0 and PTE_U == 0, evict the page (not a user page)
    else if ((*pte & PTE_U) == 0){
      delete_page_from_lru(P2V(PTE_ADDR(*pte)));
    }
    // if PTE_A == 0, evict the page
    else{
      break;
    }
  }
  release(&page_lock); // get_block_number acquires swap_space.lock

  // get block number of the victim page
  int blkno = get_block_number();
  if(blkno == -1){
    // cprintf ("reclaim: get_block_number failed\n");
    return 0;
  }

  // write the victim page to the swap space
  swapwrite(P2V(PTE_ADDR(*pte)), blkno); //P2V(PTE_ADDR(*pte)) = victim->vaddr

  // remove the victim page from the LRU list
  delete_page_from_lru(P2V(PTE_ADDR(*pte)));

  // clear the PTE
  kfree(P2V(PTE_ADDR(*pte)));
  *pte = ((*pte & 0xFFF) & (~PTE_P)) | (blkno << 12);

  // set the bitmap --> this causes some error idk lol
  // clear_bitmap(blkno);

  return 1;
}

int
get_block_number(void)
{
  if(swap_space.num_free_blocks == 0)
    return -1;
  
  acquire(&swap_space.lock);
  // find the first byte that is not all full (0xFF)  
  int i;
  for(i = 0; i < LAST_BITMAP_INDEX; i++){
    if(swap_space.bitmap[i] != (char)0xFF){
      // cprintf("found\n");
      break;
    }
  }
  // if all bytes are 0xFF, return -1
  if(i == LAST_BITMAP_INDEX){
    // cprintf("get_block_number: all bytes are 0xFF\n");
    release(&swap_space.lock);
    return -1;
  }
  // find the first bit that is not 1
  for(uint j = 0; j < 8; j++){
    if(!(swap_space.bitmap[i] & (1 << j))){
      swap_space.bitmap[i] |= (1 << j);
      release(&swap_space.lock);
      return i * 8 + j;
    }
  }

  release(&swap_space.lock);
  return -1;
}

void
set_bitmap(int blkno)
{
  acquire(&swap_space.lock);
  swap_space.num_free_blocks--;
  swap_space.bitmap[blkno / 8] |= (1 << (blkno % 8));
  release(&swap_space.lock);
}

void
clear_bitmap(int blkno)
{
  acquire(&swap_space.lock);
  swap_space.num_free_blocks++;
  swap_space.bitmap[blkno / 8] &= ~(1 << (blkno % 8));
  release(&swap_space.lock);
}

void 
add_page_to_lru(pde_t *pgdir, char *pa, char *va)
{
  uint pages_idx = V2P(pa) >> 12;

  acquire(&page_lock);
  num_lru_pages++;
  num_free_pages--;

  pages[pages_idx].pgdir = pgdir;
  pages[pages_idx].vaddr = va;

  // add to the head of the LRU list
  // if the LRU list is empty
  if(page_lru_head == 0){
    page_lru_head = &pages[pages_idx];
    pages[pages_idx].next = &pages[pages_idx];
    pages[pages_idx].prev = &pages[pages_idx];
  }
  // if the LRU list is not empty
  else{
    pages[pages_idx].next = page_lru_head; // new element points to the head
    pages[pages_idx].prev = page_lru_head->prev; // new element's prev points to the last element
    page_lru_head->prev = &pages[pages_idx]; // head's prev points to the new element
    pages[pages_idx].prev->next = &pages[pages_idx]; // last element's next points to the new element
    page_lru_head = &pages[pages_idx]; // head points to the new element
  }

  release(&page_lock);
}

void
delete_page_from_lru(char* pa)
{
  uint pages_idx = V2P(pa) >> 12;

  acquire(&page_lock);

  // if the page is not in the LRU list, return
  if(pages[pages_idx].next == 0 || pages[pages_idx].prev == 0){
    release(&page_lock);
    return;
  }

  // update the number of lru pages and free pages
  num_lru_pages--;
  num_free_pages++;

  // if the page is the only element in the LRU list
  if(pages[pages_idx].next == &pages[pages_idx] || num_lru_pages == 1){
    page_lru_head->prev = 0;
    page_lru_head->next = 0;
    page_lru_head = 0;
  }
  // if the page is the head of the LRU list
  else if(page_lru_head == &pages[pages_idx]){
    page_lru_head = pages[pages_idx].next;
    pages[pages_idx].prev->next = pages[pages_idx].next;
    pages[pages_idx].next->prev = pages[pages_idx].prev;
  }
  // if the page is the tail of the LRU list
  else if(page_lru_head->prev == &pages[pages_idx]){
    page_lru_head->prev = pages[pages_idx].prev;
    pages[pages_idx].prev->next = pages[pages_idx].next;
    pages[pages_idx].next->prev = pages[pages_idx].prev;
  }
  // if the page is not the head or tail of the LRU list
  else{
    pages[pages_idx].prev->next = pages[pages_idx].next;
    pages[pages_idx].next->prev = pages[pages_idx].prev;
  }

  // clear the page --> causes kfree to panic for some reason
  // pages[pages_idx].pgdir = 0;
  // pages[pages_idx].vaddr = 0;
  // pages[pages_idx].next = 0;
  // pages[pages_idx].prev = 0;

  memset((void*) &pages[pages_idx], 0, sizeof(struct page));

  release(&page_lock);
  return;
}