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
void print_head_help(const char *command);
void print_head_error(const char *filename);

// function implementations
int main(int argc, char *argv[]) {
    int opt;
    int num_lines = 10;
    int fd;

    while ((opt = getopt(argc, argv, "n:h")) != -1) {
        switch (opt) {
        case 'n': // set the number of lines to print
            // check if num_lines is a valid number
            if (isdigit(*optarg)) {
                num_lines = atoi(optarg);
            } else {
                fprintf(stderr, "pa2_head: invalid number of lines: %s\n", optarg);
                return -1;
            }
            break;
        case 'h': // print the help message
            print_head_help(argv[0]);
            return 0;
        default:
            fprintf(stderr, "Try 'head --help' for more information.\n");
            return -1;
        }
    }

    char buffer[BUFFER_SIZE];
    int lineCount = 0;
    int bytes_read = 0;
    // if no file is given or file is -, read stdin
    if (optind >= argc || strcmp(argv[optind], "-") == 0) {
        // [ ] check if this runs the correct amount of lines
        while ((bytes_read = read(0, buffer, 1)) > 0) {
            for (int i = 0; i < bytes_read; i++) {
                write(1, buffer, 1);

                if (buffer[i] == '\n') {
                    lineCount++;
                    if (lineCount == num_lines) break;
                }
            }

            if (lineCount == num_lines) break;
        }
    } else { // file is given
        fd = open(argv[optind], O_RDONLY);

        if (fd == -1) {
            print_head_error(argv[optind]);
            return -1;
        }

        while ((bytes_read = read(fd, buffer, 1)) > 0) {
            for (int i = 0; i < bytes_read; i++) {
                write(1, buffer, 1);
                if (buffer[i] == '\n') {
                    lineCount++;
                    if (lineCount == num_lines) break;
                }
            }

            if (lineCount == num_lines) break;
        }

        if (fd != 0) close(fd);
    }

    return 0;
}

void print_head_help(const char *command) {
    printf("pa2_head Usage: %s [-n number] [file]\n", command);
    printf("Output the first part of files\n\n");
    printf("With more than one FILE, precede each with a header giving the file name.\n\n");
    printf("Mandatory arguments to long options are mandatory for short options too.\n");
    printf("  -n, --lines=[-]NUM      output the first NUM lines instead of the first 10;\n");
    printf("                           with the leading '-', print all but the last\n");
    printf("                           NUM lines of each file\n");
    printf("  -h, --help              display this help and exit\n");
}

void print_head_error(const char *filename) {
    switch (errno) {
    case EACCES: // permission denied
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: Permission denied\n", filename);
        break;
    case EISDIR: // file is a directory
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: Is a directory\n", filename);
        break;
    case ENOENT: // file does not exist
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: No such file or directory\n", filename);
        break;
    case ENOTDIR: // not a directory
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: Not a directory\n", filename);
        break;
    case EPERM: // operation not permitted
        fprintf(stderr, "pa2_head: cannot open '%s' for reading: Operation not permitted\n", filename);
        break;
    default: // [ ]: check if this is the correct err msg for other errors
        fprintf(stderr, "pa2_head: %s\n", strerror(errno));
    }
}