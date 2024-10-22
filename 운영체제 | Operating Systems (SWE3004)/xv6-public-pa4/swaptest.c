#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


int main () {
	int a, b;
    swapstat(&a, &b);

    printf(1, "Swap space: %d/%d\n", a, b);

    // sbrk loop
    for (int i = 0; i < 30000; i++) {
        void *p = sbrk(4096);
        if (p == (void *)-1) {
            printf(1, "sbrk failed\n");
            break;
        }
    }

    swapstat(&a, &b);
    printf(1, "Swap space: %d/%d\n", a, b);

    int pid = fork();
    if(pid == 0) {
        for (int i = 0; i < 30000; i++) {
            void *p = sbrk(4096);
            if (p == (void *)-1) {
                printf(1, "sbrk failed\n");
                break;
            }
        }

        printf(1, "Child Swap space: %d/%d\n", a, b);

        exit();
    }else{
        wait();
    }

    // un-sbrk loop
    for (int i = 0; i < 60000; i++) {
        void *p = sbrk(-4096);
        if (p == (void *)-1) {
            printf(1, "sbrk failed\n");
            break;
        }
    }

    swapstat(&a, &b);
    printf(1, "Swap space: %d/%d\n", a, b);


    exit();
}

// #include "param.h"
// #include "types.h"
// #include "stat.h"
// #include "user.h"
// #include "fs.h"
// #include "fcntl.h"
// #include "syscall.h"
// #include "traps.h"
// #include "memlayout.h"


// int main () {
// 	int a, b;

//     swapstat(&a, &b);
// }
