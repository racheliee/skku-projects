#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libgen.h>

#define BUFFER_SIZE 1024
#define MAX_PATH 4096

// function prototypes
int is_directory(const char *path);
void print_cp_file_open_error(const char *filename);
void print_cp_dir_error(const char *filename);

// function implementations
int main(int argc, char *argv[]) {
    int opt;
    int src_fd; // guaranteed to be a file
    int dst_fd; // can be a file or a directory
    char buffer[BUFFER_SIZE];
    int bytes_read = 0;

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h': // print the help message
            printf("Usage: %s [file1] [file2]\n", argv[0]);
            printf("Copy the contents of file1 to file2.\n");
            printf("If file2 does not exist, create it.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("  -h, --help              display this help and exit\n");
            return 0;
        default:
            fprintf(stderr, "Try 'cp --help' for more information.\n");
            return -1;
        }
    }

    // check if there are enough arguments
    switch (argc) {
    case 1: // no arguments
        fprintf(stderr, "pa2_cp: missing file operand\n");
        return -1;
    case 2: // only one argument
        fprintf(stderr, "pa2_cp: missing destination file operand after '%s'\n", argv[1]);
        return -1;
    default:
        break;
    }

    struct stat dst_st;
    char path[MAX_PATH];
    stat(argv[2], &dst_st);

    if(argc == 3 && S_ISREG(dst_st.st_mode)){
        // open the source file
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd == -1) {
            print_cp_file_open_error(argv[1]);
        }

        // open the destination file
        dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dst_fd == -1) {
            print_cp_file_open_error(argv[2]);
        }

        // copy the file contents
        while((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0){
            if(write(dst_fd, buffer, bytes_read) == -1){
                fprintf(stderr, "pa2_cp: error writing to '%s': %s\n", argv[2], strerror(errno));
                return -1;
            }
        }

        // close the files
        if(src_fd != 0) close(src_fd);
        if(dst_fd != 0) close(dst_fd);

    } else { // when destination is a directory
        // check if the last argument is a directory
        if (stat(argv[argc - 1], &dst_st) == -1) {
            //[ ] check error message
            fprintf(stderr, "pa2_cp: cannot stat '%s': %s\n", argv[argc - 1], strerror(errno));
            return -1;
        }

        if (!S_ISDIR(dst_st.st_mode)) {
            fprintf(stderr, "pa2_cp: target '%s' is not a directory\n", argv[argc - 1]);
            return -1;
        }

        // copy each file to the directory
        for(int i = 1; i < argc-1; i++){
            // open the source file
            src_fd = open(argv[i], O_RDONLY);  
            if(src_fd == -1){
                print_cp_file_open_error(argv[i]);
                return -1;
            }

            // create a new path for the file
            char* filename = basename(argv[i]); //gets filename without the path

            char dst_path[MAX_PATH];
            // concatenate the directory path and the filename
            snprintf(dst_path, MAX_PATH, "%s/%s", argv[argc-1], filename); 

            // open the destination file
            dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(dst_fd == -1){
                print_cp_file_open_error(dst_path);
                return -1;
            }

            // copy the file contents
            while((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0){
                if(write(dst_fd, buffer, bytes_read) == -1){
                    fprintf(stderr, "pa2_cp: error writing to '%s': %s\n", dst_path, strerror(errno));
                    return -1;
                }
            }

            // close the files
            if(src_fd != 0) close(src_fd);
            if(dst_fd != 0) close(dst_fd);
        }
    }

    return 0;
}

void print_cp_file_open_error(const char *filename) {
    switch (errno) {
    case EACCES: // permission denied
        fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Permission denied\n", filename);
        break;
    case EISDIR: // file is a directory
        fprintf(stderr, "pa2_cp: cannnot open '%s' for reading: Is a directory\n", filename);
        break;
    case ENOENT: // file does not exist
        fprintf(stderr, "pa2_cp: cannot stat '%s': No such file or directory\n", filename);
        break;
    case ENOTDIR: // not a directory
        // comeback: this error is not needed probably
        fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Not a directory\n", filename);
        break;
    case EPERM: // operation not permitted
        fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Operation not permitted\n", filename);
        break;
    default:
        fprintf(stderr, "pa2_cp: %s\n", strerror(errno));
    }
}

// fixme: check error messages
void print_cp_dir_error(const char *filename){
    switch(errno){
        case EACCES:
            fprintf(stderr, "pa2_cp: cannot stat '%s': Permission denied\n", filename);
            break;
        case EISDIR:
            fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Is a directory\n", filename);
            break;
        case ENOENT:
            fprintf(stderr, "pa2_cp: cannot open '%s' for reading: No such file or directory\n", filename);
            break;
        case ENOTDIR:
            fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Not a directory\n", filename);
            break;
        case EPERM:
            fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Operation not permitted\n", filename);
            break;
        default:
            fprintf(stderr, "pa2_cp: %s\n", strerror(errno));
    }
}