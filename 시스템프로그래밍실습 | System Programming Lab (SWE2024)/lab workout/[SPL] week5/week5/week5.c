#include <errno.h>             //errno
#include <linux/limits.h>      //PATH_MAX
#include <readline/history.h>  //add_history
#include <readline/readline.h> //readline
#include <stdio.h>             //printf
#include <stdlib.h>            //free
#include <string.h>            //strtok
#include <sys/types.h>         //pid_t
#include <sys/wait.h>          //waitpid
#include <unistd.h>            //fork, execvp

#define MAX_ARGS 1000

void exec_cmd(char *args[]) {
    pid_t pid = fork();

    if (pid == -1) { // fork failed
        fprintf(stderr, "Error: Fork failed\n");
        exit(2);
    } else if (pid == 0) { // child process
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(1);
        }
    } else { // parent process
        int child_status;
        waitpid(pid, &child_status, 0); // wait for child to finish
    }
}

int main() {
    while (1) {
        // readline
        char *cmd = readline("$  ");

        if (cmd == NULL) {
            printf("Error: Failed to read input\n");
            exit(1);
        }
        add_history(cmd);

        // strtok_r
        char *args[MAX_ARGS];
        char *save_ptr;
        char *ptr = strtok_r(cmd, " ", &save_ptr);

        int i = 0;
        while (ptr != NULL) {
            if (i >= MAX_ARGS - 1) {
                printf("Error: Too many arguments\n");
                exit(2);
            }
            args[i++] = ptr;
            ptr = strtok_r(NULL, " ", &save_ptr);
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            printf("exit\n");
            if (args[1] == NULL) { // if no number is given exit with 0
                exit(0);
            } else { // otherwise exit with the given number
                exit(atoi(args[1]));
            }
        }

        exec_cmd(args); // execute commands
        free(cmd);
    }

    return 0;
}