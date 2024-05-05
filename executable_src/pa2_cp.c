#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_PATH 4096

void copy_file(const char *src, const char *dst) {
    int src_fd;
    int dest_fd;

    // open the source file
    src_fd = open(src, O_RDONLY);
    if (src_fd == -1) {
        switch (errno) {
        case EACCES:
            fprintf(stderr, "pa2_cp: cannot open '%s' for reading: Permission denied\n", src);
            break;
        case ENOENT:
            fprintf(stderr, "pa2_cp: cannot stat '%s' for reading: No such file or directory\n", src);
            break;
        default:
            fprintf(stderr, "pa2_cp: %s\n", strerror(errno));
        }
        return; // do not exit, continue with the next file
    }

    // open the destination file
    dest_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        switch (errno) {
        case EACCES:
            fprintf(stderr, "pa2_cp: cannot open '%s' for writing: Permission denied\n", dst);
            break;
        case ENOENT:
            fprintf(stderr, "pa2_cp: cannot stat '%s': No such file or directory\n", dst);
            break;
        default:
            fprintf(stderr, "pa2_cp: %s\n", strerror(errno));
        }
        close(src_fd);
        exit(1);
    }

    // copy the file contents
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            fprintf(stderr, "pa2_cp: error writing to '%s': %s\n", dst, strerror(errno));
            close(src_fd);
            close(dest_fd);
            exit(1);
        }
    }

    // close the files
    close(src_fd);
    close(dest_fd);

    return;
}

// function implementations
int main(int argc, char *argv[]) {
    int opt;

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
            fprintf(stderr, "Try 'cp -h' for more information.\n");
            return -1;
        }
    }

    // check if there are enough arguments
    switch (argc) {
    case 1: // no arguments
        fprintf(stderr, "pa2_cp: missing file operand\n");
        exit(1);
    case 2: // only one argument
        fprintf(stderr, "pa2_cp: missing destination file operand after '%s'\n", argv[1]);
        exit(1);
    default:
        break;
    }

    // stat the destination file (the last argument)
    struct stat dst_st;
    int is_directory = (stat(argv[argc - 1], &dst_st) == 0 && S_ISDIR(dst_st.st_mode));

    // loop through the source files and copy them to the destination
    for (int i = 1; i < argc - 1; ++i) {
        if (is_directory) {
            char *filename = basename(argv[i]);
            char dst_path[MAX_PATH];
            snprintf(dst_path, MAX_PATH, "%s/%s", argv[argc - 1], filename);
            copy_file(argv[i], dst_path);
        } else {
            copy_file(argv[i], argv[argc - 1]);
        }
    }

    return 0;
}
