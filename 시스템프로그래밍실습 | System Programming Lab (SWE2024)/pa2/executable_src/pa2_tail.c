#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// function prototypes
void print_tail(char *file_name, int num_lines) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    int lines_read = 0;
    int lines_to_skip = 0;
    int lines_printed = 0;

    if (strcmp(file_name, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(file_name, "r");

        // check for errors
        if (fp == NULL) {
            fprintf(stderr, "pa2_tail: cannot open '%s' for reading: %s\n", file_name, strerror(errno));
            exit(1);
        }

        // check if file is a directory
        struct stat st;
        if (stat(file_name, &st) == 0 && S_ISDIR(st.st_mode)) {
            fclose(fp);
            fprintf(stderr, "pa2_tail: cannot open '%s' for reading: Is a directory\n", file_name);
            exit(1);
        }
    }

    // count the number of lines in the file
    while (getline(&line, &len, fp) != -1) {
        lines_read++;
    }

    // calculate the number of lines to skip
    if (lines_read > num_lines) {
        lines_to_skip = lines_read - num_lines;
    }

    // reset file pointer to the beginning
    fseek(fp, 0, SEEK_SET);

    // move file pointer to the line to start printing from
    while (lines_to_skip > 0 && getline(&line, &len, fp) != -1) {
        lines_to_skip--;
    }

    // print the lines
    while (getline(&line, &len, fp) != -1) {
        printf("%s", line);
        lines_printed++;
    }

    // free memory and close file
    if (line != NULL) {
        free(line);
    }
    if (fp != stdin) {
        fclose(fp);
    }
}

// function implementations
int main(int argc, char *argv[]) {
    int opt;
    int num_lines = 10;

    while ((opt = getopt(argc, argv, "n:h")) != -1) {
        switch (opt) {
        case 'n':
            // check if num_lines is a valid number
            char *end;
            num_lines = strtol(optarg, &end, 10);

            if (*end != '\0' || num_lines < 0) {
                fprintf(stderr, "pa2_tail: invalid number of lines: '%s'\n", optarg);
                return -1;
            }

            break;
        case 'h':
            printf("pa2_tail Usage: %s [-n number] [file]\n", argv[0]);
            printf("Output the last part of files\n\n");
            printf("With more than one FILE, precede each with a header giving the file name.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("  -n, --lines=[-]NUM      output the last NUM lines instead of the last 10;\n");
            printf("                           with the leading '-', print all but the first\n");
            printf("                           NUM lines of each file\n");
            printf("  -h, --help              display this help and exit\n");
            return 0;
        default:
            fprintf(stderr, "Try 'tail -h' for more information.\n");
            return -1;
        }
    }

    // if no file is given or file is -, read stdin
    if (optind >= argc || strcmp(argv[optind], "-") == 0) {
        print_tail("-", num_lines);
    } else { // file is given
        print_tail(argv[optind], num_lines);
    }

    return 0;
}