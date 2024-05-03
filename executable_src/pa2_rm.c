#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    int opt;
    int fd;

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
    if(argc == 1){
        fprintf(stderr, "pa2_rm: missing operand\n");
        return -1;
    }

    // remove file(s)
    // no need to consider directories and protected files
    // if error occurs, other files will still be removed
    for(int i = 1; i < argc; i++){
        if(unlink(argv[i]) == -1){
            switch(errno){
                case EACCES: // permission denied
                    fprintf(stderr, "pa2_rm: cannot remove '%s': Permission denied\n", argv[i]);
                    break;
                case EISDIR: // file is a directory
                    fprintf(stderr, "pa2_rm: cannot remove '%s': Is a directory\n", argv[i]);
                    break;
                case ENOENT: // file does not exist
                    fprintf(stderr, "pa2_rm: cannot remove '%s': No such file or directory\n", argv[i]);
                    break;
                case ENOTDIR: // not a directory
                    fprintf(stderr, "pa2_rm: cannot remove '%s': Not a directory\n", argv[i]);
                    break;
                case EPERM: // operation not permitted
                    fprintf(stderr, "pa2_rm: cannot remove '%s': Operation not permitted\n", argv[i]);
                    break;
                default: 
                    fprintf(stderr, "pa2_rm: cannot remove '%s': %s\n", argv[i], strerror(errno));
            }
        }
    }

    return 0;
}