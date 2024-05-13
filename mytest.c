// PA3 test code (mmap) ========================================================================================================
#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "fcntl.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "proc.h"
#include "syscall.h"

int main(void) {
    int fd;
    int size = 4096; // comeback: if not aligned, error should occur

    // uint mmap(uint addr, int length, int prot, int flags, int fd, int offset)
    // int munmap(uint addr)
    // int freemem(void)

    printf(1, "init free memory: %d\n\n", freemem());
    
    // anonymous mapping ========================================================================================================
    // case 1: with MAP_POPULATE
    char *t1 = (char *) mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_POPULATE, -1, 0);

    if(t1 == (char *)0 || t1 == (char *)-1){
        printf(1, "t1 mmap failed\n");
        exit();
    }

    printf(1, "t1 mmap success\n");
    printf(1, "free memory after t1 mmap: %d\n", freemem());

    t1[0] = 'a'; // comeback: try without PROT_WRITE
    t1[1] = 'b';
    t1[2] = 'c';

    printf(1, "\nread from t1:\n");
    for(int i = 0; i < size; i++){
        printf(1, "%c", t1[i]);
    }
    printf(1, "\n\n");

    // case 2: without MAP_POPULATE; page fault occurs
    char *t2 = (char *) mmap(size, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);

    if(t2 == (char *)0 || t2 == (char *)-1) {
        printf(1, "t2 mmap failed\n");
        exit();
    }

    printf(1, "t2 mmap success\n");
    printf(1, "free memory after t2 mmap: %d\n", freemem());

    // should call page fault handler
    t2[0] = 'a';
    t2[1] = 'b';
    t2[2] = 'c';    

    printf(1, "\nread from t2:\n");
    for(int i = 0; i < size; i++){
        printf(1, "%c", t2[i]);
    }
    printf(1, "\n\n");


    // file mapping  ========================================================================================================
    // case 1: with MAP_POPULATE
    fd = open("README", O_RDWR);

    char *t3 = (char *) mmap(size*2, size, PROT_READ, MAP_POPULATE, fd, 0);

    if (t3 == (char *)0 || t3 == (char *)-1) {
        printf(1, "t3 mmap failed\n");
        exit();
    }

    printf(1, "t3 mmap success\n");
    printf(1, "\nfree memory after t3 mmap: %d\n", freemem());

    printf(1, "\nread from file:\n");
    for(int i = 0; i < size; i++){
        printf(1, "%c", t3[i]);
    }

    printf(1, "\n\n");

    // // case 2: without MAP_POPULATE; page fault occurs
    // char *t4 = (char *) mmap(size*3, size, PROT_READ, 0, fd, 0);

    // if (t4 == (char *)0 || t4 == (char *)-1) {
    //     printf(1, "t4 mmap failed\n");
    //     exit();
    // }

    // printf(1, "t4 mmap success\n");
    // printf(1, "free memory after t4 mmap: %d\n", freemem());

    // printf(1, "\nread from file:\n");
    // for(int i = 0; i < size; i++){
    //     printf(1, "%c", t4[i]);
    // }
    // printf(1, "\n\n");

    // munmap test ========================================================================================================
    // unmap t1
    printf(1, "munmap t1: %d\n", munmap(MMAPBASE));
    printf(1, "free memory after t1 munmap: %d\n\n", freemem());

    // unmap t2
    // printf(1, "munmap t2: %d\n", munmap(MMAPBASE + size));
    // printf(1, "free memory after t2 munmap: %d\n\n", freemem());

    // unmap t3
    printf(1, "munmap t3: %d\n", munmap(MMAPBASE + size*2));
    printf(1, "free memory after t3 munmap: %d\n\n", freemem());

    // unmap t4
    // printf(1, "munmap t4: %d\n", munmap(MMAPBASE + size*3));
    // printf(1, "free memory after t4 munmap: %d\n\n", freemem());

    close(fd);

    // fork test ========================================================================================================
    int pid = fork();

    if(pid < 0){
        printf(1, "fork failed\n");
        exit();
    }else if(pid == 0){ // child process
        printf(1, "child process\n");
        
        printf(1, "free memory in child process: %d\n", freemem());
        printf(1, "munmap t1 in child process: %d\n", munmap(MMAPBASE));

        t2[3] = 'd';
        printf(1, "\nread from t2 in child process:\n");
        for(int i = 0; i < size; i++){
            printf(1, "%c", t2[i]);
        }

        printf(1, "\nfree memory after t1 munmap in child process: %d\n", freemem());
        printf(1, "munmap t2 in child process: %d\n", munmap(MMAPBASE + size));
    }else{
        wait();
        printf(1, "\nparent process\n");

        printf(1, "free memory in parent process: %d\n", freemem());
        printf(1, "munmap t1 in parent process: %d\n", munmap(MMAPBASE));

        printf(1, "free memory after t1 munmap in parent process: %d\n", freemem());
        printf(1, "munmap t2 in parent process: %d\n", munmap(MMAPBASE + size));
    }

    return 0;
}

// PA2 test code (scheduler) ========================================================================================================
// #include "types.h"
// #include "stat.h"
// #include "user.h"

// void busy_work() {
//     double val = 1.0;
//     while (1) {
//         val = val * 1.000001 + 0.000001;  // Just a random calculation
//     }
// }

// int main(void) {
//     int pid;
//     printf(1, "Testing runnable state\n");

//     pid = fork();
//     if (pid < 0) {
//         printf(2, "Fork failed\n");
//         exit();
//     }

//     // Child process
//     if (pid == 0) {
//         setnice(getpid(), 5);  // Set nice value to 10
//         busy_work();
//         exit();
//     }

//     // Parent process
//     setnice(getpid(), 0);  // Set nice value to 1
//     uint j = 0;
//     int count = 0;
//     while (1) {
//         j++;
//         if (j % 10000000000 == 0) {  // swui에서 돌릴거면 10^9, lab에서 돌릴거면 10^10
//             ps(0);
//             printf(1, "\n");
//             count++;
//         }
//         if (count == 5) {
//             kill(pid);
//             wait();
//             printf(1, "\nTest complete\n");
//             break;
//         }
//     }

//     exit();
// }

// PA1 test code (syscalls) ========================================================================================================
// #include "types.h"
// #include "user.h"
// #include "stat.h"

// int main() {
//     int i;

//     fork();

//     ps(0);
//     setnice(1,5);
//     ps(0);

//     wait();

//     // for (i = 1; i <= 10; i++) {
//     //     printf(1, "%d: ", i);
//     //     if (getpname(i)) {
//     //         printf(1, "getpname failed\n");
//     //     }
//     // }
//     // setnice(1,5);
//     // for (i = 1; i < 11; i++) {
//     //     printf(1, "%d: ", i);
//     //     if (getpname(i)) {
//     //         printf(1, "getpname failed\n");
//     //     }
//     // }

//     exit();
// }