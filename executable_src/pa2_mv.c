#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_PATH 4096

int main(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "h")) != -1){
        switch(opt){
            case 'h':
                fprintf(stdout, "Usage: mv [FILE] [DESTINATION]\n");
                fprintf(stdout, "Rename the FILE to DESTINATION.\n");
                fprintf(stdout, "Mandatory arguments to long options are mandatory for short options too.\n");
                fprintf(stdout, "  -h, --help     display this help and exit\n");
                return 0;
            default:
                fprintf(stderr, "Try 'mv --help' for more information.\n");
                return -1;
        }
    }

    // check if there are enough arguments
    switch(argc){
        case 1:
            fprintf(stderr, "pa2_mv: missing file operand\n");
            return -1;
        case 2:
            fprintf(stderr, "pa2_mv: missing destination file operand after '%s'\n", argv[1]);
            return -1;
        default:
            break;
    }

    struct stat statbuf;
    char path[MAX_PATH];
    
    // [ ] this was by copilot lmao
    stat(argv[2], &statbuf);
    if (S_ISDIR(statbuf.st_mode))
    {
        char *base_name = basename(argv[1]);
        snprintf(path, sizeof(path), "%s/%s", argv[2], base_name);
    } else {
        strncpy(path, argv[2], sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
    }

    // check if source and destination are the same
    if(strcmp(argv[1], path) == 0){
        fprintf(stderr, "pa2_mv: '%s' and '%s' are the same file\n", argv[1], path);
        return -1;
    }

    if (rename(argv[1], path) == -1) { // fixme: path에 대한 에러처리가 안되어잇음
        switch (errno)
        { // fixme: directory cannot be accessed도 추가해야함
            case EACCES:
                fprintf(stderr, "pa2_mv: Permission denied\n");
                break;
            case EISDIR:
                fprintf(stderr, "pa2_mv: Is a directory\n");
                break;
            case ENOENT: // file does not exist
                fprintf(stderr, "pa2_mv: cannot stat '%s': No such file or directory\n", argv[1]);
                break;
            case ENOTDIR:
                fprintf(stderr, "pa2_mv: Not a directory\n");
                break;
            case EPERM:
                fprintf(stderr, "pa2_mv: Operation not permitted\n");
                break;
            case EINVAL: // any invalid argument, when newpath is a subdirectory of oldpath
                // fixme:  subdirectory error needs to be done but einval takes care of all invalid args
                fprintf(stderr, "pa2_mv: Invalid argument\n");
                break;
            case ENOTEMPTY:
                fprintf(stderr, "pa2_mv: Directory not empty\n");
                break;
            case EEXIST:
                fprintf(stderr, "pa2_mv: File exists\n");
                break;
            default:
                fprintf(stderr, "pa2_mv: %s\n", strerror(errno));
                break;
        }
        return -1;
    }

    return 0;
}