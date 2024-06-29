// #include "types.h"
// #include "stat.h"
// #include "user.h"


// void testcfs()
// {
// 	int parent = getpid();
// 	int child;
// 	int i;
// 	double x = 0, z;
	
// 	if((child = fork()) == 0) { // child
// 		setnice(parent, 9);		// if you set parent's priority lower than child, 
// 								// 2nd ps will only printout parent process,
// 								// since child finished its job earlier & exit
// 		for(i = 0; i < 3000; i++){
// 			for ( z = 0; z < 30000.0; z += 0.1 )
// 				x =  x + 3.14 * 89.64;
// 		}
// 		ps(0);
// 		exit();
// 	} else {	
// 		setnice(child, 0);	  //parent
// 		for(i = 0; i < 3000; i++){
// 			for ( z = 0; z < 30000.0; z += 0.1 )
// 				x =  x + 3.14 * 89.64;
// 		}
// 		ps(0);
// 		wait();
// 	}
// }
// int main(int argc, char **argv)
// {
// 		printf(1, "=== TEST START ===\n");
// 		testcfs();
// 		printf(1, "=== TEST   END ===\n");
// 		exit();

// }


#include "types.h"
#include "stat.h"
#include "user.h"

void busy_work() {
    double val = 1.0;
    while (1) {
        val = val * 1.000001 + 0.000001;  // Just a random calculation
    }
}

int main(void) {
    int pid;
    printf(1, "Testing runnable state\n");

    pid = fork();
    if (pid < 0) {
        printf(2, "Fork failed\n");
        exit();
    }

    // Child process
    if (pid == 0) {
        setnice(getpid(), 5);  // Set nice value to 10
        busy_work();
        exit();
    }

    // Parent process
    setnice(getpid(), 0);  // Set nice value to 1
    uint j = 0;
    int count = 0;
    while (1) {
        j++;
        if (j % 10000000000 == 0) {  // swui에서 돌릴거면 10^9, lab에서 돌릴거면 10^10
            ps(0);
            printf(1, "\n");
            count++;
        }
        if (count == 5) {
            kill(pid);
            wait();
            printf(1, "\nTest complete\n");
            break;
        }
    }

    exit();
}

// #include "types.h"
// #include "stat.h"
// #include "user.h"

// #define NCHILD 2

// void busy_work() {
//     double val = 1.0;
//     // int i = 500000000;
//    while(1) {
//     val = val * 1.000001 + 0.000001; // Just a random calculation
// }
// }

// int main(void) {
//     int n, pid;
//     printf(1, "Testing runnable state\n");


//     pid = fork();
//     if(pid < 0) {
//         printf(2, "Fork failed\n");
//         exit();
//     }

//     setnice(1,5);
//     setnice(2,5);
//     setnice(3,5);
//     setnice(4,0);
//     setnice(5,0);

//     // Child process
//     if(pid == 0) {
//         busy_work();
//         exit();
//     }
    


//     // Now, we assume that some children are in the runnable state.
//     // It's time to call our ps syscall to check the process states.
//     sleep(100);
//     long j = 50000;
//     while (j--) {
//         double val = 1.0;
//         val = val * 1.000001 + 0.000001; // Just a random calculation
//         printf(1," ");
//     }
//     printf(1, "\n");
//     ps(0);

//     // kill the child process
//     kill(pid);
//     wait();


//     printf(1, "Test complete\n");
//     exit();
// }

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