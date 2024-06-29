#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h': // print the help message
            fprintf(stdout, "Usage: rm [FILE]...\n");
            fprintf(stdout, "Remove (unlink) the FILE(s).\n");
            fprintf(stdout, "Mandatory arguments to long options are mandatory for short options too.\n");
            fprintf(stdout, "  -h, --help     display this help and exit\n");
            return 0;
        default:
            fprintf(stderr, "Try 'rm --help' for more information.\n");
            return -1;
        }
    }

    // check if there are enough arguments
    if (argc == 1) {
        fprintf(stderr, "pa2_rm: missing operand\n");
        exit(1);
    }

    // remove file(s)
    // if error occurs, other files will still be removed
    for (int i = 1; i < argc; i++) {
        if (unlink(argv[i]) == -1) {
            fprintf(stderr, "pa2_rm: cannot remove '%s': %s\n", argv[i], strerror(errno));
        }
    }

    return 0;
}