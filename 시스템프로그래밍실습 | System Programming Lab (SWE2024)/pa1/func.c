#include "func.h"

// global variables ==========================================
long long int line_number;
int bad_char[MAX_CHAR];
char file_buffer[FILE_BUFFER_SIZE];
ssize_t file_bytes_read;
char pattern_buffer[PATTERN_BUFFER_SIZE];
ssize_t pattern_bytes_read;
char line_buffer[FILE_BUFFER_SIZE];

// string functions ==========================================
int str_len(char *str) {
    char *s;
    for (s = str; *s && *s != '\n' && *s != '\r' ; ++s)
        ;
    return (s - str);
}

int str_cmp(char *str1, char *str2) {
    while (*str1 == *str2++) {
        if (*str1++ == 0) {
            return 0;
        }
    }
    return (*(unsigned char *)str1 - *(unsigned char *)--str2);
}

char *str_chr(char *s, int ch) {
    char c;

    c = ch;
    for (;; ++s) {
        if (*s == c)
            return ((char *)s);
        if (*s == '\0')
            return (NULL);
    }
}

int str_tok(char delim, char pattern_arr[][MAX_WORD_LENGTH]) {
    int i, j = 0, k = 0;
    int pattern_len = str_len(pattern_buffer);

    for (i = 0; i < pattern_len; i++) {
        if (pattern_buffer[i] != delim) {
            pattern_arr[k][j++] = pattern_buffer[i];
        } else {
            if (j > 0) {
                pattern_arr[k][j] = '\0';
                k++;
                j = 0;
            }
        }
    }

    if (j > 0) {
        pattern_arr[k][j] = '\0';
        k++;
    }

    return k;
}

char *to_lower(char *str) {
    char *s;
    for (s = str; *s; ++s) {
        if (*s >= 'A' && *s <= 'Z') {
            *s = *s + 32;
        }
    }
    return str;
}

int is_blank(char c) {
    if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
        return 1;
    }
    return 0;
}

int is_alpha(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    return 0;
}

// helper functions ==========================================
int get_num_digits(long long num) {
    if (num == 0)
        return 1;

    int count = 0;
    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}

// write an int to stdout
void writeNumber(long long num) {
    char line_num[15];
    int num_len = get_num_digits(num);

    int temp = num;
    for (int i = num_len - 1; i >= 0; i--) {
        line_num[i] = temp % 10 + '0'; // convert to char
        temp /= 10;
    }

    write(1, line_num, num_len); // write line number
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// remove word from line
void remove_word(int pattern_len, int shift) {
    int i = shift;
    while (pattern_len--) {
        line_buffer[i] = ' ';
        i++;
    }
}

// check input pattern and return the case number
// case 1: single word, case 2: multiple words, case 3: quoted string, case 4: * in between
int check_pattern_case(int *num_tokens, char pattern_arr[][MAX_WORD_LENGTH]) {
    to_lower(pattern_buffer);

    if (pattern_buffer[0] == '"') { // case 3
        // remove quotes
        int i;
        for (i = 0; i < pattern_bytes_read - 2; i++) {
            pattern_buffer[i] = pattern_buffer[i + 1];
        }
        pattern_buffer[i - 1] = '\0';

        return 3;
    } else if (str_chr(pattern_buffer, ' ') != NULL && pattern_buffer[0] != '"') { // case 2
        *num_tokens = str_tok(' ', pattern_arr);
        return 2;
    } else if (str_chr(pattern_buffer, '*') != NULL) { // case 4
        *num_tokens = str_tok('*', pattern_arr);
        return 4;
    } else { // case 1
        return 1;
    }
}

// read file and search for search pattern
void read_file(int fd, int caseNum, int num_tokens, char pattern_arr[][MAX_WORD_LENGTH]) {
    int line_buf_index = 0; 

    while ((file_bytes_read = read(fd, file_buffer, FILE_BUFFER_SIZE)) > 0) {
        // in case file does not end with newline char, replace the last char with newline char
        if (file_bytes_read != FILE_BUFFER_SIZE && (file_buffer[file_bytes_read - 1] != '\n')) {
            file_buffer[file_bytes_read] = '\n';
            file_bytes_read++;
        }

        int i;
        for (i = 0; i < file_bytes_read; i++) {
            while (i < file_bytes_read && file_buffer[i] != '\n') {
                line_buffer[line_buf_index++] = file_buffer[i++];
            }

            if (file_buffer[i] == '\n') {
                line_buffer[line_buf_index] = '\0';
                to_lower(line_buffer);

                if (caseNum == 1 || caseNum == 3) {
                    search_pattern(caseNum, pattern_buffer, str_len(pattern_buffer));
                } else if (caseNum == 4) { // case 4
                    int first_index = search_pattern(caseNum, pattern_arr[0], str_len(pattern_arr[0]));
                    int second_index = search_pattern(caseNum, pattern_arr[1], str_len(pattern_arr[1]));

                    if (first_index != -1 && second_index != -1 && first_index < second_index) {
                        int alpha_count = 0;

                        // check if there is a word in between
                        for (int k = first_index + str_len(pattern_arr[0]); k < second_index; k++) {
                            if (is_alpha(line_buffer[k])) {
                                alpha_count++;
                                break;
                            }
                        }
                        if (alpha_count > 0) {
                            writeNumber(line_number);
                            write(1, " ", 1);
                        }
                    }
                    // if the second word comes before the first, check if there is a second word behind the first
                    // or if both words are the same, remove the first word and research for the second word index
                    else if ((first_index != -1 && second_index != -1 && first_index > second_index) || (str_cmp(pattern_arr[0], pattern_arr[1]) == 0)) {
                        int second_len = str_len(pattern_arr[1]);
                        remove_word(second_len, second_index);
                        second_index = search_pattern(caseNum, pattern_arr[1], second_len);

                        if (second_index != -1 && first_index < second_index) {
                            writeNumber(line_number);
                            write(1, " ", 1);
                        }
                    }
                } else { // case 2
                    int k;
                    for (k = 0; k < num_tokens; k++) {
                        if (search_pattern(caseNum, pattern_arr[k], str_len(pattern_arr[k])) == -1) {
                            break;
                        }
                    }

                    if (k == num_tokens) {
                        writeNumber(line_number);
                        write(1, " ", 1);
                    }
                }

                line_number++;
                line_buf_index = 0;
            }
        }
    }

    write(1, "\n", 1); // print newline after the last line
}

// boyer-moore functions ====================================
void bad_char_table(char *pattern, int len) {
    // initialize bad_char table
    for (int i = 0; i < MAX_CHAR; ++i) {
        bad_char[i] = -1;
    }

    // fill in bad_char table
    for (int i = 0; i < len; ++i) {
        bad_char[(int)pattern[i]] = i;
    }
}

int search_pattern(int caseNum, char *pattern, int pattern_len) {
    bad_char_table(pattern, pattern_len);

    int shift = 0, found = 0;
    int line_len = str_len(line_buffer);

    while (shift <= (line_len - pattern_len)) {
        int j = pattern_len - 1;

        while (j >= 0 && pattern[j] == line_buffer[shift + j]) {
            j--;
        }

        if (j < 0) { // pattern found
            if ((shift == 0 || is_blank(line_buffer[shift - 1])) && (shift + pattern_len == line_len || is_blank(line_buffer[shift + pattern_len]))) {
                if (caseNum == 1 || caseNum == 3) { // case 1 and 3 print out line num and index
                    writeNumber(line_number);
                    write(1, ":", 1);
                    writeNumber(shift);
                    write(1, " ", 1);
                    found = 1;
                } else if (caseNum == 2) { // case 2
                    remove_word(pattern_len, shift);
                    return 1;
                } else { // case 4
                    return shift;
                }
            }
            shift += (shift + pattern_len < line_len) ? pattern_len - bad_char[(int)line_buffer[shift + pattern_len]] : 1;
        } else {
            shift += max(1, j-bad_char[(int)line_buffer[shift+j]]);
        }
    }

    if (found) {
        return 1;
    }
    return -1;
}