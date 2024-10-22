#include "func.h"

int main(int argc, char *argv[]) {
    // check if file name has been entered
    if (argc < 2) {
        write(1, "Please enter a file name\n", 26);
        exit(1);
    }

    // read file name & open og.txt
    char *filename = argv[1];

    // open file
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        write(1, "Error reading file name\n", 24);
        exit(1);
    }

    int caseNum, num_tokens;
    while (1) {
        line_number = 1; // reset line number

        pattern_bytes_read = read(STDIN_FILENO, pattern_buffer, PATTERN_BUFFER_SIZE); // read search word

        if (pattern_bytes_read == -1) {
            write(1, "Error reading pattern\n", 22);
            exit(1);
        }

        pattern_buffer[pattern_bytes_read - 1] = '\0'; // remove newline character

        if (str_cmp(pattern_buffer, "PA1EXIT") == 0) { // end program if pattern is "PA1EXIT"
            break;
        } else if (pattern_buffer[0] == '\0' || pattern_buffer[0] == '\n') { // skip if pattern is empty
            continue;
        }

        // check pattern
        char pattern_arr[PATTERN_BUFFER_SIZE/2][MAX_WORD_LENGTH];
        caseNum = check_pattern_case(&num_tokens, pattern_arr);

        // read file
        read_file(fd, caseNum, num_tokens, pattern_arr);

        // reset file pointer
        if (lseek(fd, 0, SEEK_SET) == -1) {
            write(1, "Error resetting file pointer\n", 30);
            exit(1);
        }
    }

    close(fd);
    return 0;
}