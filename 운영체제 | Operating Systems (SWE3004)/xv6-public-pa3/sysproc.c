#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// getpname (pa1 example sys call)
int
sys_getpname(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return getpname(pid);
}

// getnice 
int 
sys_getnice(void)
{
  int pid;

  // check if pid value has been passed
  if(argint(0, &pid) < 0)
    return -1;
  return getnice(pid);
}

// setnice 
int 
sys_setnice(void)
{
  int pid, value;

  // check if both pid and value have been passed
  if(argint(0, &pid) < 0 || argint(1, &value) < 0)
    return -1;
  return setnice(pid, value);
}

// ps
void
sys_ps(void)
{
  int pid;

  // check if pid value has been passed
  if(argint(0, &pid) < 0)
    return;
  return ps(pid);
}

// mmap
int
sys_mmap(void)
{
  uint addr;
  int len, prot, flags, fd, offset;
  
  // check if all values have been passed
  if(arguint(0, &addr) < 0)
    return -1;
  if(argint(1, &len) < 0 || argint(2, &prot) < 0 || argint(3, &flags) < 0 || argint(4, &fd) < 0 || argint(5, &offset) < 0)
    return -1;
  return mmap(addr, len, prot, flags, fd, offset);
}

// munmap
int
sys_munmap(void)
{
  uint addr;

  // check if all values have been passed
  if(arguint(0, &addr) < 0)
    return -1;
  return munmap(addr);
}

// freemem
int 
sys_freemem(void)
{
  return freemem();
}