#include "types.h"
#include "user.h"
#include "stat.h"

int main()
{
    // int i;

    // int pid, val;
 
    // for(i = 1; i < 11; i++){
    //     printf(1, "%d: ", i);
    //     if(getpname(i)){
    //        printf(1, "Wrong pid\n");
    //     }
    // }

    // // check getnice
    // for(i = -1; i < 11; i++){
    //     printf(1, "pid %d's nice: %d\n", i, getnice(i));
    // }

    // // check setnice
    // for(i = -1; i < 11; i++){
    //     setnice(i, i+1);
    //     printf(1, "pid %d's new nice: %d\n", i, getnice(i));
    // }
    // ps(0);

    // wrong nice val
    // val = -40;
    // setnice(1, val);
    // printf(1, "pid %d's new nice: %d\n", 1, getnice(1));
    // ps(1);
    
    // val = 40;
    // setnice(1, val);
    // printf(1, "pid %d's new nice: %d\n", 1, getnice(1));
    // ps(1);

    // //check ps
    // for(i = -1; i < 11; i++){
    //     ps(i);

    // }

    // for(i = -1; i < 11; i++){
    //     printf(1, "%d: ", i);
    //     if(getpname(i)){
    //        printf(1, "Wrong pid\n");
    //     }
    // }
    getpname(0);
    setnice(0, 21);
    ps(0);
    printf(1, "0's nice: %d\n", getnice(0));
    printf(1, "0's name: ");
    getpname(0);


    exit();
}