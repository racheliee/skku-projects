#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

uint total_ticks = 0; // counts the total ticks
uint weight[40] = {/*   0*/ 88761,   71755,    56483,    46273,    36291,
                  /*   5*/ 29154,   23254,    18705,    14949,    11916,
                  /*  10*/ 9548,    7620,     6100,     4904,     3906,
                  /*  15*/ 3121,    2501,     1991,     1586,     1277,
                  /*  20*/ 1024,    820,      655,      526,      423,
                  /*  25*/ 335,     272,      215,      172,      137,
                  /*  30*/ 110,     87,       70,       56,       45,
                  /*  35*/ 36,      29,       23,       18,       15};

struct mmap_area mmap_area[64] = {0}; // mmap_area array

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  //initialize values
  p->nice = 20;
  p->runtime = 0;
  p->vruntime = 0;

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();

    // initialize mmap_area array
  for(int i = 0; i < MMAP_NUM; i++){
    mmap_area[i].f = 0;
    mmap_area[i].addr = 0;
    mmap_area[i].length = 0;
    mmap_area[i].offset = 0;
    mmap_area[i].prot = 0;
    mmap_area[i].flags = 0;
    mmap_area[i].p = 0;
    mmap_area[i].in_use = 0;
    mmap_area[i].uses_page_table = 0;
  }
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  //set nice value to 20
  p->nice = 20;

  //set runtime, current runtime, and vruntime to 0
  p->runtime = 0;
  p->vruntime = 0;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  np->nice = curproc->nice; // inherit nice value from parent
  np->runtime = curproc->runtime; // inherit runtime from parent
  np->vruntime = curproc->vruntime; // inherit vruntime from parent

  pid = np->pid;

  // copy mmap_area array ========================================
  // find parent process's mmap_area
  // parent mmap: mmap_area[i]
  // child mmap: mmap_area[j]
  for(int i = 0; i < MMAP_NUM; i++){
    if(mmap_area[i].in_use == 1 && mmap_area[i].p == curproc){
      // look for child process's mmap_area
      int j;
      for(j = 0; j < MMAP_NUM; j++){
        // if the area is in use, continue
        if(mmap_area[j].in_use == 1) continue;

        // copy parent process's mmap_area to child process
        if(mmap_area[i].f != 0) {
          mmap_area[j].f = filedup(mmap_area[i].f);
          mmap_area[j].f->off = mmap_area[i].offset;
        }
        else mmap_area[j].f = 0;
        mmap_area[j].addr = mmap_area[i].addr;
        mmap_area[j].length = mmap_area[i].length;
        mmap_area[j].offset = mmap_area[i].offset;
        mmap_area[j].prot = mmap_area[i].prot;
        mmap_area[j].flags = mmap_area[i].flags;
        mmap_area[j].p = np;
        mmap_area[j].in_use = 1;
        mmap_area[j].uses_page_table = mmap_area[i].uses_page_table;

        // copy page table if needed (based on copyuvm from vm.c)
        if(mmap_area[j].uses_page_table == 1){
          for(uint k = 0; k < mmap_area[j].length; k+= PGSIZE){
            // find the pte of the parent process's page table
            pte_t *pte = walkpgdir(np->pgdir, (void *)(mmap_area[j].addr + k), 0);

            if(pte == 0) continue; // pte does not exist
            if((*pte & PTE_P) == 0) continue; // pte is not present

            char* mem = kalloc();
            if(mem == 0) return -1; // memory allocation failed

            // reset memory
            memset(mem, 0, PGSIZE);

            // copy the contents of the parent process's page table to the child process's page table
            memmove(mem, (char *)P2V(PTE_ADDR(*pte)), PGSIZE);

            // map the child process's page table to the child process's page table
            mappages(np->pgdir, (void *)(mmap_area[j].addr + k), PGSIZE, V2P(mem), PTE_U | mmap_area[j].prot);
          } // end of for loop
        }

        // if a mmap_area is found for the child process, break
        break;
      } // end of for loop looking for child process's mmap_area

      // if no free mmap areas for child were found
      if(j == MMAP_NUM){
        // cprintf("no empty mmap_area for child process\n");
        return -1;
      }

    } // end of if statement that matches parent process's mmap_area
  } // end of for loop looking for parent process's mmap_area
  // ============================================================

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

    uint min_vruntime = __UINT32_MAX__; // minimum vruntime
    struct proc *min_vrun_p = 0; // process with minimum vruntime
    int runnable_exists = 0; // check if runnable processes exist
    uint total_weight = 0; // total weight of runnable processes

    // Loop over process table to find process with min vruntime
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;
      
      // update min_vruntime and min_vrun_p
      if(p->vruntime < min_vruntime){
        min_vruntime = p->vruntime;
        min_vrun_p = p;
      }
      runnable_exists = 1;
      total_weight += weight[p->nice];
    }

    // if there is a runnable process, swtich to the process with min vruntime
    if(runnable_exists == 1){
      //calculate timeslice of the process that will be scheduled
      min_vrun_p->timeslice = 1000 * 10 * weight[min_vrun_p->nice] / total_weight;

      // switch to chosen process
      c->proc = min_vrun_p;
      switchuvm(min_vrun_p);
      min_vrun_p->state = RUNNING;
      
      swtch(&(c->scheduler), min_vrun_p->context);
      switchkvm();

      // process is done running for now
      c->proc =0;
    }
    release(&ptable.lock);

    /*// Loop over process table looking for process to run.
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);*/

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  struct proc *p = myproc();

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  myproc()->state = RUNNABLE;
  // add the runtime to the total runtime
  // myproc()->runtime += myproc()->cur_runtime;
  // calculate vruntime
  // myproc()->vruntime += myproc()->cur_runtime * weight[20] / weight[myproc()->nice];
  // reset current runtime
  // myproc()->cur_runtime = 0;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  //check if runnable processes exist
  int runnable_exists = 0;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == RUNNABLE){
      runnable_exists = 1;
      break;
    }
  }

  //set vruntime to 0 if there are no runnable processes
  if(runnable_exists == 0){
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state == SLEEPING && p->chan == chan){
        p->vruntime = 0; 
        p->state = RUNNABLE;
      }
    }
  }else{
    // find minimum vruntim of processes in the ready queue
    uint min_vruntime = __UINT32_MAX__; // minimum vruntime
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state == RUNNABLE && p->vruntime < min_vruntime){
        min_vruntime = p->vruntime;
      }
    }

    // set vruntime to min vruntime - vruntime(1000 millitick)
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state == SLEEPING && p->chan == chan){
        p->vruntime = min_vruntime - (1000*weight[20]/weight[p->nice]);
        p->state = RUNNABLE;
      }
    }
  }
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

// getpname (pa1 example sys call)
int
getpname(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      cprintf("%s\n", p->name);
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

// getnice 
// function obtains the nice value of a process
// return nice value of target process on success
// return -1 if there is no process corresponding to the pid
int
getnice(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      release(&ptable.lock);
      return p->nice;
    }
  }
  release(&ptable.lock);
  return -1;
}

// setnice 
// function sets the nice value of a process
// return 0 on success
// return -1 if there no process corresponding to the pid or the nice value is invalid
int
setnice(int pid, int value)
{
  struct proc *p;

  //check new nice value's range
  if(value < 0 || value > 39) return -1;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      // set new nice value
      p->nice = value;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}


// calculate the length of a number
int
digit_len(uint num){
  int len = 1;
  while(num >= 10){
    num /= 10;
    len++;
  }
  return len;
}


// ps
// prints out process' information (includes name, pid, state, and priority(nice value))
// if pid is 0, print out all processes' information
// if there is no process corresponding to the pid, print out nothing
// no return value
void 
ps(int pid)
{
  struct proc *p;

  //  procsate values in string format
  char *procstate_str[] = {"UNUSED", "EMBRYO", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"};

  acquire(&ptable.lock);
  if(pid > 0){ //pid is not 0, print out the process info
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->pid == pid){
        cprintf("name            pid        state         priority     runtime/weight   runtime      vruntime      tick "); //space: 16 11 16 13 17 13
        cprintf("%d000\n", total_ticks); //print in milliticks
        
        cprintf("%s", p->name);
        int bound = 16-strlen(p->name);
        for(int i = 0; i < bound; i++) cprintf(" ");
        
        cprintf("%d", p->pid);
        int pid_length = 1;
        int temp_pid = p->pid;
        while (temp_pid >= 10){
          temp_pid /= 10;
          pid_length++;
        }
        bound = 11-pid_length;
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%s", procstate_str[p->state]);
        bound = 14-strlen(procstate_str[p->state]);
        for(int i = 0;i < bound; i++) cprintf(" ");

        cprintf("%d", p->nice);
        if(p->nice > 9) bound = 11;
        else bound = 12;
        for(int i = 0;i < bound; i++) cprintf(" ");

        cprintf("%u", p->runtime/weight[p->nice]);
        bound = 17 - digit_len(p->runtime/weight[p->nice]);
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%u", p->runtime);
        bound = 13 - digit_len(p->runtime);
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%u\n", p->vruntime);

      }
    }
  }else{ //print out all process' information
     cprintf("name            pid        state         priority     runtime/weight   runtime      vruntime      tick "); //space: 16 11 16 13 17 13
    cprintf("%d000\n", total_ticks); //print in milliticks
    
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != UNUSED){
        // cprintf("%s         %d        %s          %d\n", p->name, p->pid, procstate_str[p->state], p->nice);
        cprintf("%s", p->name);
        int bound = 16-strlen(p->name);
        for(int i = 0; i < bound; i++) cprintf(" ");
        
        cprintf("%d", p->pid);
        int pid_length = 1;
        int temp_pid = p->pid;
        while (temp_pid >= 10){
          temp_pid /= 10;
          pid_length++;
        }
        bound = 11-pid_length;
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%s", procstate_str[p->state]);
        bound = 14-strlen(procstate_str[p->state]);
        for(int i = 0;i < bound; i++) cprintf(" ");

        cprintf("%d", p->nice);
        if(p->nice > 9) bound = 11;
        else bound = 12;
        for(int i = 0;i < bound; i++) cprintf(" ");

        cprintf("%u", p->runtime/weight[p->nice]);
        bound = 17 - digit_len(p->runtime/weight[p->nice]);
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%u", p->runtime);
        bound = 13 - digit_len(p->runtime);
        for(int i = 0; i < bound; i++) cprintf(" ");

        cprintf("%u\n", p->vruntime);
      }
    }
  }
  
  release(&ptable.lock);
  return;
}

// mmap
// 1. addr is always page-aligned (MMAPBASE + addr is the start addr)
// 2. length is always multiple of page size (MMAPBASE + addr + length is the end addr)
// 3. prot is either PROT_READ or PROT_WRITE or both (prot should path with file's open flag)
// 4. flags
//  - MAP_ANONYMOUS: anonymous mapping
//  - MAP_ANONYMOUS not given: file mapping
//  - MAP_POPULATE: allocate physical page & make page table for whole mapping area
//  - MAP_POPULATE not given: just record its mapping area
// 5. fd is given for file mappings, if not, it should be -1
// 6. offset is given for file mappings, if not, it should be 0
// fail: returns 0
//  1. not anonymous but fd = -1
//  2. protection of the file & prot of the parameter are different
//  3. address args must be page aligned, if not return 0
//  - mapping area is overlapped is not considered
//  - if additional errors occur, we will let you know by writing notification
uint mmap(uint addr, int length, int prot, int flags, int fd, int offset){
  // 1. if not anonymous but fd = -1
  // (flags & MAP_ANONYMOUS) == 0: file mapping, (flags & MAP_ANONYMOUS) != 0: anonymous mapping
  if(fd == -1 && (flags & MAP_ANONYMOUS) == 0){
    // cprintf("not anonymous but fd = -1\n");
    return 0;
  }

  // if no flags are given, treat it as MAP_ANONYMOUS
  if(flags == 0){
    flags = MAP_ANONYMOUS;
  }

  // MAP_ANONYMOUS이면서 fd와 offset이 -1과 0이 아닌 경우에 mmap은 0을 반환
  if((flags & MAP_ANONYMOUS) && (fd != -1 || offset != 0)){
    // cprintf("anonymous mapping but fd != -1 or offset != 0\n");
    return 0;
  }

  // 3. address args must be page aligned, if not return 0
  if(addr % PGSIZE != 0 || length % PGSIZE != 0){
    // cprintf("address args must be page aligned\n");
    return 0;
  }

  struct proc *curproc = myproc();
  struct file *file = 0;
  
  // get file with given fd (no invalid fd's are given)
  if(fd != -1) file = curproc->ofile[fd]; 

  // 2. if file mapping, check protection of the file & prot of the parameter are different
  // (prot & PROT_READ) == 0: no read permission, (prot & PROT_WRITE) == 0: no write permission
  if((flags & MAP_ANONYMOUS) == 0){
    // if read and write protection doesn't match
    // shift file->writable to the left by 1 to match the prot_write value
    if(((prot & PROT_READ) != file->readable) || ((prot & PROT_WRITE) && file->writable == 0)){
      // cprintf("protection of the file & prot of the parameter are different\n");
      return 0;
    }
  }

  // find empty mmap_area from the mmap_area array
  struct mmap_area *new_mmap_area = 0;
  for(int i = 0; i < MMAP_NUM; i++){
    if(mmap_area[i].in_use == 0){
      new_mmap_area = &mmap_area[i];
      break;
    }
  }

  // no empty mmap_area found
  if(new_mmap_area == 0){
    // cprintf("no empty mmap_area\n");
    return 0;
  }

  // initialize new mmap_area with given values and current process
  if(file != 0) new_mmap_area->f = filedup(file);
  new_mmap_area->addr = addr + MMAPBASE;
  new_mmap_area->length = length;
  new_mmap_area->offset = offset;
  new_mmap_area->prot = prot;
  new_mmap_area->flags = flags;
  new_mmap_area->p = curproc;
  new_mmap_area->in_use = 1;

  // 1. private file / anonymous mapping with MAP_POPULATE
  if((flags & MAP_POPULATE)){
    // cprintf("private file mapping with MAP_POPULATE\n");
    uint end_addr = MMAPBASE + addr + length; // end address of the mapping area

    for(uint i = MMAPBASE + addr; i < end_addr; i += PGSIZE){
      // allocate physical page
      char* new_page = kalloc();

      // memory allocation failed
      if(new_page == 0) {
        // cprintf("memory allocation failed\n");
        new_mmap_area->in_use = 0;
        return 0;
      }
      
      // initialize the allocated memory
      memset(new_page, 0, PGSIZE);
      
      // read file content to the allocated memory
      if((flags & MAP_ANONYMOUS) == 0){
        file->off = offset; // set the offset of the file
        fileread(file, new_page, PGSIZE);
      }

      // make page table
      if(mappages(curproc->pgdir, (void*)i, PGSIZE, V2P(new_page), PTE_U | prot) < 0){
        // cprintf("mappages failed\n");
        new_mmap_area->in_use = 0;
        kfree(new_page);
        return 0;
      }
    }

    new_mmap_area-> uses_page_table = 1;
    return addr + MMAPBASE;
  }
  // 2. private file / anonymous mapping without MAP_POPULATE
  else if(!(flags & MAP_POPULATE)){
    new_mmap_area->uses_page_table = 0;

    return addr + MMAPBASE;
  }

  return 0;
}

// munmap
// return 1 on success, -1 on failure
// 1. addr will be alwys given with the start of the memory region which is page aligned
// 2. munmap() should remove corresponding mmap_structure (if none, -1)
// 3. if physical page is allocated & page table is constructed, should free physical page & page table
// 4. if physical page is not allocated (page fault has not been occured), just remove mmap_area structure
// 5. In one mmap_area, situation of some pages are allocated and some are not can happen
int munmap(uint addr){
  // if addr is not page aligned, return 0
  // comeback: check
  if(addr % PGSIZE != 0){
    // cprintf("addr is not page aligned\n");
    return 0;
  }

  // find mmap_area with given addr
  struct mmap_area *target_mmap_area = 0;
  struct proc *curproc = myproc();

  for(int i = 0; i < MMAP_NUM; i++){
    if(mmap_area[i].addr == addr && mmap_area[i].p == curproc){
      target_mmap_area = &mmap_area[i];
      break;
    }
  }

  // if target is not found, return -1
  if(target_mmap_area == 0){
    // cprintf("target mmap_area not found\n");
    return -1;
  }

  // if physical page is allocated & page table is constructed, should free physical page & page table
  if(target_mmap_area->uses_page_table){
    // have to free length/PGSIZE pages
    for(uint i = 0; i < target_mmap_area->length; i += PGSIZE){
      pte_t* pte = walkpgdir(curproc->pgdir, (void*)(addr + i), 0); // 0 since we don't want to allocate a new page

      // if pte is not allocated, continue
      if(pte == 0) continue;
      // if pte is not present, continue
      if((*pte & PTE_P) == 0) continue;

      // free physical page (based on deallocuvm in vm.c)
      uint pa = PTE_ADDR(*pte);
      if(pa == 0) panic("kfree");
      char* v = P2V(pa);
      memset(v, 1, PGSIZE);
      kfree(v);

      // clear page table entry
      *pte = 0;
    }
  }

  // decrease the reference count of the file
  if(target_mmap_area->f != 0 && target_mmap_area->f->ref > 0){
    target_mmap_area->f->ref--;
  } 

  // remove mmap_area structure
  target_mmap_area->f = 0;
  target_mmap_area->addr = 0;
  target_mmap_area->length = 0;
  target_mmap_area->offset = 0;
  target_mmap_area->prot = 0;
  target_mmap_area->flags = 0;
  target_mmap_area->p = 0;
  target_mmap_area->in_use = 0;
  target_mmap_area->uses_page_table = 0;

  return 1;
}

// freemem
// syscall to return current number of free memory pages
// 1. when kernel frees (put page into free list), freemem should increase
// 2. when kernel allocates (take page from free list and give to process), free mem shoudl decrease
int freemem(void){
  return get_num_of_free_pages(); // in kalloc.c & defs.h
}

// pagefault handler
// catches pagefault (interrupt I4, T_PGFLT)
// determine fault addr by reading CR2 reg (using rcr2()) & access was read or write
//  - read: tf->err & 2 == 0, write: tf->err & 2 == 1
// find according mapping region in mmap_area
//  - if not found, return -1
// if fault was write and the region is read-only, return -1
// else, allocate physical page, fill with 0
//  - if file mapping, read file into physical page w offset
//  - if anonymous, leave the page as 0s
// make page table entry
int pagefault_handler(uint err){
  uint fault_addr = rcr2();
  // cprintf("pagefault handler: err: %x\n", err);
  // if(err != 2){
  //   return 1;
  // }
  // err = 5 : segmentation fault

  struct proc *curproc = myproc();
  struct mmap_area *cur_mmap_area = 0;

  // cprintf("pagefault handler: fault_addr: %x\n", fault_addr);

  // find the mmap_area with the fault address
  for(int i = 0; i < MMAP_NUM; i++){
    if(mmap_area[i].in_use == 1 && mmap_area[i].p == curproc && mmap_area[i].addr <= fault_addr && fault_addr < mmap_area[i].addr + mmap_area[i].length){
      cur_mmap_area = &mmap_area[i];
      break;
    }
  }

  // mmap_area with fault address not found
  if(cur_mmap_area == 0){
    // cprintf("pagefault handler: mmap_area with fault address not found\n");
    curproc->killed = 1;
    return -1;
  }

  // write error and region is read-only (writing flag not given)
  if((err & 2) && (cur_mmap_area->prot & PROT_WRITE) == 0){
    // cprintf("pagefault handler: write error and region is read-only\n");
    curproc->killed = 1;
    return -1;
  }

  // allocate physical page
  char* new_mem = kalloc();
  if(new_mem == 0) {
    curproc->killed = 1;
    return -1;
  }

  // initialize the allocated memory
  memset(new_mem, 0, PGSIZE);


  // read file content to the allocated memory if its a file mapping
  if((cur_mmap_area->flags & MAP_ANONYMOUS) == 0){
    // set file offset
    cur_mmap_area->f->off = cur_mmap_area->offset + (fault_addr - cur_mmap_area->addr);
    // read file
    fileread(cur_mmap_area->f, new_mem, PGSIZE);
  }

  // make page table
  uint align_addr = fault_addr - (fault_addr % PGSIZE);
  if(mappages(curproc->pgdir, (void*)align_addr, PGSIZE, V2P(new_mem), PTE_U | cur_mmap_area->prot) < 0){
    // cprintf("pagefault handler: mappages failed\n");
    curproc->killed = 1;
    kfree(new_mem);
    return -1;
  }

  // update page table status
  cur_mmap_area->uses_page_table = 1;

  return 1;
}