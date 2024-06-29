#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <getopt.h>

#define BUFFER_SIZE 1024

void print_head(char *filename, int num_lines) {
    FILE *fp;
    char* line = NULL;
    size_t len = 0;
    int lines_printed = 0;

    // open file or stdin
    if (strcmp(filename, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(filename, "r");

        if(fp == NULL){
            fprintf(stderr, "pa2_head: cannot open '%s' for reading: %s\n", filename, strerror(errno));
            exit(1);
        }

        struct stat st;
        if(stat(filename, &st) == 0 && S_ISDIR(st.st_mode)){
            fclose(fp);
            fprintf(stderr, "pa2_head: cannot open '%s' for reading: Is a directory\n", filename);
            exit(1);
        }
    }

    // read and print the first num_lines lines
    while(lines_printed < num_lines && getline(&line, &len, fp) != -1){
        printf("%s", line);
        lines_printed++;
    }


    // free memory and close file
    if(line != NULL){
        free(line);
    }
    if(fp != stdin){
        fclose(fp);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int num_lines = 10;

    while ((opt = getopt(argc, argv, "n:h")) != -1) {
        switch (opt) {
        case 'n': 
            // check if num_lines is a valid number
            char* end;
            num_lines = strtol(optarg, &end, 10);

            if (*end != '\0' || num_lines < 0) {
                fprintf(stderr, "pa2_head: invalid number of lines: %s\n", optarg);
                return -1;
            }

            break;
        case 'h': // print the help message
            printf("pa2_head Usage: %s [-n number] [file]\n", argv[0]);
            printf("Output the first part of files\n\n");
            printf("With more than one FILE, precede each with a header giving the file name.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("  -n, --lines=[-]NUM      output the first NUM lines instead of the first 10;\n");
            printf("                           with the leading '-', print all but the last\n");
            printf("                           NUM lines of each file\n");
            printf("  -h, --help              display this help and exit\n");
            return 0;
        default:
            fprintf(stderr, "Try 'head -h' for more information.\n");
            return -1;
        }
    }

    // if no file is given or file is -, read stdin
    if (optind >= argc || strcmp(argv[optind], "-") == 0) {
        print_head("-", num_lines);
    } 
    // file is given
    else { 
        print_head(argv[optind], num_lines);
    }

    return 0;
}