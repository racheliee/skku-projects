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

void move_file(const char *src, const char *dst){
    if(rename(src, dst) == -1){
        switch(errno){
            case EACCES:
                fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': Permission denied\n", src, dst);
                break;
            case ENOENT:
                fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': No such file or directory\n", src, dst);
                break;
            case EPERM:
                fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': Operation not permitted\n", src, dst);
                break;
            default:
                fprintf(stderr, "pa2_mv: cannot move '%s' to '%s': %s\n", src, dst, strerror(errno));
        }
        return; // do not exit, continue with the next file
    }
}

int is_dest_subdir(const char *src, const char *dst){
    char src_path[MAX_PATH];
    char dst_path[MAX_PATH];
    realpath(src, src_path);
    realpath(dst, dst_path);

    return strstr(dst_path, src_path) != NULL;
}

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
                fprintf(stderr, "Try 'mv -h' for more information.\n");
                exit(1);
        }
    }

    // check if there are enough arguments
    switch(argc){
        case 1:
            fprintf(stderr, "pa2_mv: missing file operand\n");
            exit(1);
        case 2:
            fprintf(stderr, "pa2_mv: missing destination file operand after '%s'\n", argv[1]);
            exit(1);
        default:
            break;
    }


    // stat the destination file (the last argument)
    const char *dest = argv[argc - 1];
    struct stat dst_st;
    struct stat src_st;
    int is_directory = (stat(dest, &dst_st) == 0 && S_ISDIR(dst_st.st_mode));

    for(int i = 1; i < argc -1; ++i){
        // check if source and destination are the same
        if(strcmp(argv[i], dest) == 0){
            fprintf(stderr, "pa2_mv: '%s' and '%s' are the same file\n", argv[i], dest);
            continue; // move to the next file without exit
        }

        // check if source file exists
        if(stat(argv[i], &src_st) == -1){
            switch(errno){
                case ENOENT:
                    fprintf(stderr, "pa2_mv: cannot stat '%s': No such file or directory\n", argv[i]);
                    break;
                default:
                    fprintf(stderr, "pa2_mv: cannot stat '%s': %s\n", argv[i], strerror(errno));
            }
            continue; // move to the next file without exit
        }

        // check if destination is a subdirectory of the source
        if(is_dest_subdir(argv[i], dest)){
            fprintf(stderr, "pa2_mv: cannot move '%s' to a subdirectory of itself, '%s'\n", argv[i], dest);
            continue; // move to the next file without exit
        }

        // move the file
        if(is_directory){
            char *filename = basename(argv[i]);
            char dst_path[MAX_PATH];
            snprintf(dst_path, MAX_PATH, "%s/%s", dest, filename);
            move_file(argv[i], dst_path);
        }else{
            move_file(argv[i], dest);
        }
    }

    return 0;
}