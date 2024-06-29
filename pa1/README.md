# System Programming Lab PA1: File Search Utility

## Overview
This project involves creating a file search utility program that performs various search functions on a text file. The application will read a text file, process user input to search for specific words or phrases, and return the occurrences in the specified format. The program utilizes Unix File I/O and data structures.

## Objective
- **Implement a File Search Utility**: Develop a C program that can read a text file and perform basic search functions based on user input.
- **Key Requirements**:
  - Search for single or multiple words.
  - Search for consecutive words enclosed in double quotes.
  - Search using simple regular expressions with an asterisk (*) wildcard.

## Key Features

### Functionalities
- **Single Word Search**:
  - Search for all occurrences of a single word in the file using the Boyer-Moore algorithm.
  - Output format: `[line number]:[start index of the word]`.
- **Multiple Word Search**:
  - Search for lines containing all specified words using the Boyer-Moore algorithm.
  - Word order does not matter.
  - Output format: `[line number]`.
- **Consecutive Words Search**:
  - Search for lines containing the specified phrase enclosed in double quotes using the Boyer-Moore algorithm.
  - Output format: `[line number]:[start index of the phrase]`.
- **Wildcard Search**:
  - Search for lines containing words separated by any number of words (using `word1*word2` format) using the Boyer-Moore algorithm.
  - Output format: `[line number]`.

### Specifications
- **Program Requirements**:
  - Read input text file name from command line arguments.
  - Continuously wait for user input and perform searches until "PA1EXIT" is entered.
  - Handle files larger than system memory by processing line by line.
  - Print results sequentially from the top of the file.
  - Use only specified header files and follow given constraints.

### Algorithm Details
- **Boyer-Moore Algorithm**:
  - Implemented for efficient string searching.
  - **Bad Character Rule**: Utilized for all searches.
  - **Good Suffix Rule**: Not included as the runtime was better without it in the context of this assignment.

### Program Flow
- **Start Program**: Read the input text file name from the command line.
- **User Input**: Continuously wait for user input to perform searches.
- **Search Functions**: Execute the appropriate search function based on the input.
- **Output Results**: Print search results in the specified format.
- **Exit Program**: Terminate the program when "PA1EXIT" is entered.

### Restrictions
- Implement the program in a Linux environment.
- Use only the allowed header files: `unistd.h`, `fcntl.h`, `stdlib.h`, `sys/types.h`, `sys/stat.h`, `errno.h`.
- The executable file should be named `pa1`.

### Example Usage
```bash
$ ./pa1 input.txt
word
1:0 2:5 3:10
"word phrase"
1:0 1:15
word1*word2
2
PA1EXIT
```
