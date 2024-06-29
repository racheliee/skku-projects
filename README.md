# System Programming Lab Project Assignments

This repository contains implementations of several system programming assignments, each organized into separate folders. Each assignment folder includes its implementation files and documentation, meeting course requirements and coding standards.

# Projects Included

## PA1: File Search Utility
- **Objective:** Develop a C program that reads a text file and performs various search functions based on user input.
- **Grade:** 92.5/100
- **Key Features:** 
  - Single word search, multiple word search, consecutive words search, and wildcard search.
  - Implements the Boyer-Moore algorithm for efficient string searching.

## PA2: Mini-Shell
- **Objective:** Implement a mini-shell program supporting basic shell commands, job control, I/O redirection, and background processing.
- **Grade:** 92.5/100
- **Key Features:**
  - Command execution using `execvp`.
  - Built-in commands: `bg`, `fg`, `jobs`, `cd`, `pwd`, `exit`.
  - Self-implemented executables: `head`, `tail`, `cat`, `cp`, `mv`, `rm`.

## PA3: Reservation Server
- **Objective:** Design and implement a multi-threaded reservation server that supports many clients.
- **Grade:** 82.64/100
- **Key Features:**
  - Server manages the reservation procedure for each client.
  - Client sends queries to the server for actions like login, booking, confirming booking, canceling booking, and logout.
  - Supports concurrent handling of multiple clients.

## Lab Assignments
This repository also includes lab assignments covering topics such as FILE I/O, I/O multiplexing, daemon processes, multi-threading, IPC, and debugging with gdb.
