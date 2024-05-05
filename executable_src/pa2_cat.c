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

void print_file(const char *filename) {
    FILE *fp;

    // open file or stdin
    if (strcmp(filename, "-") == 0) {
        fp = stdin;
    } else {
        fp = fopen(filename, "r");

        if (fp == NULL) {
            fprintf(stderr, "pa2_cat: %s: No such file or directory\n", filename);
            exit(1);
        }

        // check if file is a directory
        struct stat st;
        if (stat(filename, &st) == 0 && S_ISDIR(st.st_mode)) {
            fclose(fp);
            fprintf(stderr, "pa2_cat: %s: Is a directory\n", filename);
            exit(1);
        }
    }

    // print the file
    int c;
    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
        fflush(stdout);
    }

    // close file
    if (fp != stdin) {
        fclose(fp);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h': //help option
            printf("pa2_cat Usage: %s [file1] [file2] ...\n", argv[0]);
            printf("Concatenate FILE(s) to standard output.\n\n");
            printf("With no FILE, or when FILE is -, read standard input.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("  -h, --help     display this help and exit\n");
            return 0;
        default:
            fprintf(stderr, "pa2_cat: invalid option -- '%c'\n", opt);
            fprintf(stderr, "Try 'cat -h' for more information.\n");
            return -1;
        }
    }

    // if file is not given, read from stdin
    if (argc == 1 || !strcmp(argv[1], "-")) {
        print_file("-");
    } 
    // print all files (multiple files if given)
    else {
        for (int i = optind; i < argc; i++)
            print_file(argv[i]);
    }

    return 0;
}