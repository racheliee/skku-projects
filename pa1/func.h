#ifndef FUNC_H //guard
#define FUNC_H //guard

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Constants
#define FILE_BUFFER_SIZE 12288
#define PATTERN_BUFFER_SIZE 4100
#define MAX_CHAR 256
#define MAX_WORD_LENGTH 100

// Global Variables
extern long long int line_number;
extern int bad_char[];
extern int* good_suffix;
extern char file_buffer[];
extern ssize_t file_bytes_read;
extern char pattern_buffer[];
extern ssize_t pattern_bytes_read;
extern char line_buffer[];

// Function Prototypes
//string related functions
int str_len(char *str);
int str_cmp(char *str1, char *str2);
char *str_chr(char *s, int ch);
int str_tok(char delim, char pattern_arr[][MAX_WORD_LENGTH]);
char *to_lower(char *str);
int is_blank(char c);
int is_alpha(char c);

//helper functions
int get_num_digits(long long num);
void writeNumber(long long num);
int max(int a, int b);
void remove_word(int pattern_len, int shift);

//pattern and file reading functions
int check_pattern_case(int *num_tokens, char pattern_arr[][MAX_WORD_LENGTH]);
void read_file(int fd, int caseNum, int num_tokens, char pattern_arr[][MAX_WORD_LENGTH]);

//pattern search functions
int search_pattern(int caseNum, char *pattern, int pattern_len);
void bad_char_table(char *pattern, int len);
void good_suffix_table(char *pattern, int len);

#endif // FUNC_H