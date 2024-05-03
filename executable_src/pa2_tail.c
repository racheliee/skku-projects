#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// function prototypes
int pa2_tail(int argc, char *argv[]);
void print_tail_help(const char *command);
void print_tail_error(const char *filename);

// function implementations
int pa2_tail(int argc, char *argv[]) {
    int opt;
    int num_lines = 10;
    int fd;

    while ((opt = getopt(argc, argv, "n:h")) != -1) {
        switch (opt) {
        case 'n':
            // check if num_lines is a valid number
            if (isdigit(*optarg)) {
                num_lines = atoi(optarg);
            } else {
                fprintf(stderr, "pa2_tail: invalid number of lines: %s\n", optarg);
                return -1;
            }
            break;
        case 'h':
            print_tail_help(argv[0]);
            return 0;
        default:
            fprintf(stderr, "Try 'tail --help' for more information.\n");
            return -1;
        }
    }

    char buffer[BUFFER_SIZE];
    int lineCount = 0;
    int bytes_read = 0;

    // setting the file descriptor
    // if no file is given or file is -, read stdin
    if (optind >= argc || strcmp(argv[optind], "-") == 0) {
        fd = 0;
    } else { // file is given
        fd = open(argv[optind], O_RDONLY);

        if (fd == -1) {
            print_tail_error(argv[optind]);
            return -1;
        }
    }

    // seek the end of the file
    off_t file_size = lseek(fd, 0, SEEK_END);
    if(file_size == -1) {
        fprintf(stderr, "pa2_tail: Unable to determine file size\n");
        if(fd != 0) {
            close(fd);
        }
        return -1;
    }

    // read the file from the end
    off_t offset = 0; // offset from the end of the file
    while(offset < file_size && lineCount < num_lines){
        // read the file in chunks of BUFFER_SIZE
        // calculate the read offset which is the offset from the end of the file
        off_t read_off = file_size - offset - BUFFER_SIZE;
        if(read_off < 0){
            read_off = 0;
        }

        // calculate the number of bytes to read
        ssize_t bytes_to_read = file_size - offset - read_off;
        if(bytes_read > BUFFER_SIZE){
            bytes_read = BUFFER_SIZE;
        }

        // seek to the read offset
        if(lseek(fd, read_off, SEEK_SET) == -1){
            fprintf(stderr, "pa2_tail: Unable to seek to offset %ld\n", read_off);
            if(fd != 0){
                close(fd);
            }
            return -1;
        }   

        // read the file
        ssize_t bytes_being_read = read(fd, buffer, bytes_to_read);
        if(bytes_being_read == -1){
            fprintf(stderr, "pa2_tail: Unable to read file\n");
            if(fd != 0){
                close(fd);
            }
            return -1;
        }   

        // write the file
        for(int i = bytes_being_read - 1; i >= 0; i--){
            write(1, buffer + i, 1);
            if(buffer[i] == '\n'){
                lineCount++;
                if(lineCount == num_lines) break;
            }
        }

        offset += bytes_being_read; // update the offset
    }

    if(fd != 0) close(fd); // close the file descriptor

    return 0;
}


void print_tail_help(const char *command) {
    printf("pa2_tail Usage: %s [-n number] [file]\n", command);
    printf("Output the last part of files\n\n");
    printf("With more than one FILE, precede each with a header giving the file name.\n\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("  -n, --lines=[-]NUM      output the last NUM lines instead of the last 10;\n");
    printf("                           with the leading '-', print all but the first\n");
    printf("                           NUM lines of each file\n");
    printf("  -h, --help              display this help and exit\n");
}

void print_tail_error(const char *filename) {
    switch (errno) {
    case EACCES: // permission denied
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: Permission denied\n", filename);
        break;
    case EISDIR: // file is a directory
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: Is a directory\n", filename);
        break;
    case ENOENT: // file does not exist
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: No such file or directory\n", filename);
        break;
    case ENOTDIR: // not a directory
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: Not a directory\n", filename);
        break;
    case EPERM: // operation not permitted
        fprintf(stderr, "pa2_tail: cannot open '%s' for reading: Operation not permitted\n", filename);
        break;
    default: //[ ]: check if this is the correct err msg for other errors
        fprintf(stderr, "pa2_tail: %s\n", strerror(errno));
    }
}