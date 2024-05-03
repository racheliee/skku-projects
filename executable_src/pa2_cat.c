#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int opt;
    while((opt = getopt(argc, argv, "h")) != -1){
        switch(opt){
            case 'h':
                printf("pa2_cat Usage: %s [file1] [file2] ...\n", argv[0]);
                printf("Concatenate FILE(s) to standard output.\n\n");
                printf("With no FILE, or when FILE is -, read standard input.\n\n");
                printf("Mandatory arguments to long options are mandatory for short options too.\n");
                printf("  -h, --help     display this help and exit\n");
                return 0;
            default:
                fprintf(stderr, "Try 'cat --help' for more information.\n");
                return -1;
        }
    }
    
    
    char buffer[1024];
    int fd;

    if(argc == 1 || !strcmp(argv[0], "-")) { // read from stdin 
        // [ ] check if this is an infinite loop
        while(read(0, buffer, 1) > 0)
            write(1, buffer, 1);
    } else {
        // read from file (multiple files possible)
        for(int i = 1; i < argc; i++){
            fd = open(argv[i], O_RDONLY);
            
            // error handling
            if(fd == -1){
                switch(errno){
                    case EACCES: // permission denied
                        fprintf(stderr, "pa2_cat: %s: Permission denied\n", argv[i]);
                        break;
                    case EISDIR: // file is a directory
                        fprintf(stderr, "pa2_cat: %s: Is a directory\n", argv[i]);
                        break;
                    case ENOENT: // file does not exist
                        fprintf(stderr, "pa2_cat: %s: No such file or directory\n", argv[i]);
                        break;
                    case ENOTDIR: // not a directory
                        fprintf(stderr, "pa2_cat: %s: Not a directory\n", argv[i]);
                        break;
                    case EPERM: // operation not permitted
                        fprintf(stderr, "pa2_cat: %s: Operation not permitted\n", argv[i]);
                        break;
                    default: 
                        fprintf(stderr, "pa2_cat: %s: %s\n", argv[i], strerror(errno));
                }

                return -1; // exit with error
            }

            while(read(fd, buffer, 1) > 0){
                write(1, buffer, 1);
            }

            if(fd != 0) close(fd);
        }
    }
    
    return 0;
}
