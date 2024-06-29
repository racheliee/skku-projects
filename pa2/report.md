# System Programming Lab - PA2 Report
2022310853 박연우

## 1. Objective
&nbsp; &nbsp; The goal of this assignment is to implement a mini-shell program that supports basic shell commands. The shell should be able to execute commands, handle signals, job control, support I/O redirection, and support background processing. 

&nbsp; &nbsp; The first goal is to implement 6 executables found in GNU Core Utilities: `head`, `tail`, `cat`, `cp`, `mv`, and `rm`. The second goal was is to implement 6 built-in commands: `bg`, `fg`, `jobs`, `cd`, `pwd`, and `exit`. 

## 2. How to compile and run
In the `pa2` directory, run the following commands:
```bash
make pa2
```
This will compile the source code and create an executable file named `pa2` in the `bin` directory. To run the shell, execute the following command:
```bash
cd bin
./pa2
```
To exit the shell, type `exit` or press `Ctrl + D`.

To clean the compiled files, run the following command in the `pa2` directory:
```bash
make clean
```

## 3. Shell Implementation & Design
### 3.1 Shell Structure
&nbsp; &nbsp; The shell is implemented in the `pa2_shell.c` file. The shell program is divided into the following parts:
- **Main Loop**: The main loop of the shell that continuously prompts the user for input and executes the command.
- **Parsing Input**: The shell will parse the input command and arguments using the `strtok` function. The shell will also check for I/O redirection and background processing.
- **Executing Command**: The shell will execute the command using the `execvp` function. The shell will also handle I/O redirection and background processing.
- **Signal Handling**: The shell will handle `SIGCHLD` signal using the `signal` function.
- **Job Control**: The shell will manage jobs using the `Job` struct. The shell will keep track of the jobs using a linked list.
- **Built-in Commands**: The shell will support built-in commands such as `bg`, `fg`, `jobs`, `cd`, `pwd`, and `exit`.
- **Self-Implemented Executables**: The shell will support self-implemented executables such as `head`, `tail`, `cat`, `cp`, `mv`, and `rm`.

> Note: `pa2_shell` does not support multiple output redirections for a single process (aka. tee processes) <br>
> For instance, `cat < t1.txt > t2.txt | grep a` will only redirect the output of `cat` to `t2.txt` and not to the pipe (`grep a`).

#### 3.1.1 Job Struct
&nbsp; &nbsp; The `Job` struct is used to store information about a job. The `Job` struct contains the following fields:
- **pgid**: The process group ID of the job.
- **job_num**: The job number of the job.
- **status**: The status of the job (Running, Stopped, or Done).
- **next**: A pointer to the next job in the linked list.
- **first_process**: A pointer to the first process in the job.
- **flags**: Five flags to indicate if the job is in the background, has an ampersand, is piped, is redirected, and is appending to a file.
- **recency**: The recency of the job (used for `fg` and `bg` commands).
- **input_file**: The input file for the job.
- **output_file**: The output file for the job.

#### 3.1.2 Process Struct
&nbsp; &nbsp; The `Process` struct is used to store information about a process. The `Process` struct contains the following fields:
- **pid**: The process ID of the process.
- **status**: The status of the process (Running, Stopped, or Done).
- **next**: A pointer to the next process in the linked list.
- **argv**: An array of arguments for the process.
- **argc**: The number of arguments for the process.
- **input_file**: The input file for the process.
- **output_file**: The output file for the process.
- **is_append**: A flag to indicate if the output file should be appended.
- **is_pipe**: A flag to indicate if the process is piped.

#### 3.1.3 Other Structs & Enums
- Structure `Token` is used to store the tokenized input command and arguments.
- Enum `Status` is used to define the status of a job or process.
- Enum `TokenType` is used to define the type of token (Command, Argument, Redirection, etc)

### 3.2 Global Variables
- **`first_job`**: A pointer to the first job in the linked list of jobs.
- **`job_num`**: The job number to be assigned to the next job.
- **`shell_terminal`**: The terminal file descriptor.
- **`shell_pgid`**: The process group ID of the shell.
- **`shell_name`**: The name of the shell (pa2_shell).
- **`recency`**: The recency of the last job that was stopped or done.
- **`status`**: A simple array used to print the status of a job or process.
- **`done_buffer`**: A buffer used to store the output of a process that has finished.

### 3.3 Main & Shell's Main Loop
&nbsp; &nbsp; The shell is initialized in the `main` function. The main loop of the shell is implemented in the `main` function in `pa2.c`. The shell will continuously prompt the user for input and execute the command. The shell will exit when the user types `exit` or presses `Ctrl + D` (other signals are ignored). The shell handles SIGCHLD signals to reap zombie processes. The command is read and executed in the `exec_cmd` function. Any done background jobs are to be printed.

### 3.4 Parsing Input
&nbsp; &nbsp; The shell will parse the input command and arguments. The shell will also check for I/O redirection and background processing. The parsed input will be stored as a series of processes in a `Process` struct, which is saved in the linked list in the `Job` struct.

### 3.5 Executing Command
&nbsp; &nbsp; If the command is a built-in command, the shell will execute the built-in command. If the command is a self-implemented executable, the shell will execute the executable. If the command is a system command, the shell will fork a child process and execute the command using the `execvp` function. The shell will also handle I/O redirection and background processing. The shell will wait for the child process to finish if the command is not run in the background.

### 3.6 Job Control
&nbsp; &nbsp; The shell will manage jobs using the `Job` struct. The shell will keep track of the jobs using a linked list. 

### 3.7 Signal Handling
&nbsp; &nbsp; The shell will handle signals `SIGCHLD` using the `signal` function in order to reap zombie processes. The shell will ignore other signals. 

## 4. Shell Code Explanation
&nbsp; &nbsp; Code explanation on built-in commands are explained in the following sections. This section will explain all other parts of the shell code-`pa2.c`.

### 4.1 `main` Function
&nbsp; &nbsp; The main function initialized the shell (`init_shell`), sets up the signal handlers, and enters the main loop of the shell. The main loop of the shell continuously prompts the user for input and executes the command. The shell will exit when the user types `exit` or presses `Ctrl + D` (other signals are ignored). The command is read and executed in the `exec_cmd` function. Any done background jobs are to be printed.

### 4.2 Shell Initialization/Signal Handling
1. **init_shell()**:
   - **Purpose**: Initializes the shell.
   - Initializes the shell by setting the terminal process group ID, signal handlers, and other global variables.

2. **sig_handler(int signo)**:
   - **Purpose**: Signal handler for the shell.
   - **Input**: An integer representing the signal number.
   - **Output**: No return value. Handles the signal.
   - Handles the `SIGCHLD` signal to reaping zombie processes. If a child process has finished/stopped/exited, the job status is updated and the job information is printed if necessary.


### 4.3 Command Lexing and Parsing
1. **is_special_char(char c)**:
   - **Purpose**: Determines if a character is a special character used in tokenizing commands.
   - **Input**: A character `c` to be checked.
   - **Output**: Returns 1 if the character is special, 0 otherwise.

2. **is_pipe(Token token)**:
   - **Purpose**: Checks if a token represents a pipe.
   - **Input**: A `Token` structure representing a token.
   - **Output**: Returns 1 if the token type is `TOKEN_PIPE`, 0 otherwise.

3. **is_redirection(Token token)**:
   - **Purpose**: Checks if a token represents a redirection (input, output, or append).
   - **Input**: A `Token` structure representing a token.
   - **Output**: Returns 1 if the token type is `TOKEN_REDIRECT_IN`, `TOKEN_REDIRECT_OUT`, or `TOKEN_REDIRECT_APPEND`, 0 otherwise.

4. **lex(char *cmd, Token *tokens)**:
   - **Purpose**: Tokenizes a command string into tokens.
   - **Input**: 
     - `cmd`: A string representing the command to be tokenized.
     - `tokens`: An array of `Token` structures to store the tokens.
   - **Output**: No return value. Modifies the `tokens` array with the parsed tokens.

5. **parse(Token *tokens)**:
   - **Purpose**: Parses tokens into a job structure.
   - **Input**: An array of `Token` structures representing tokens of a command.
   - **Output**: Returns a pointer to a `Job` structure representing the parsed job.
   - Iterates through the tokens and creates a `Process` structure for each process in the job. The processes are linked together in a linked list. The job is then created and returned.

### 4.4 Executing Command & Launching Jobs & Job Control
1.  **evaluate_cmd(char* cmd)**:
    - **Purpose**: Evaluates a command and executes it.
    - **Input**: A string representing the command.
    - **Output**: No return value. Executes the command.
    - Evaluates the command by checking if the command is empty or if the command is a comment. If the command is not empty, the command is tokenized and parsed. The job is then launched.

2. **exec_cmd(int argc, char *arg[])**:
    - **Purpose**: Executes a non-built-in command.
    - **Input**: 
      - `argc`: Number of command arguments.
      - `arg[]`: Array of strings representing command arguments.
    - **Output**: No return value. Executes the specified command.
    - If the command is a built-in command, the corresponding built-in command function is called. If not, the command will be executed via execvp. The command is executed in the `launch_job` function.

3. **launch_job(Job *job, int foreground)**:
    - **Purpose**: Launches a job.
    - **Input**: 
      - `job`: A pointer to a `Job` structure representing the job to be launched.
      - `foreground`: A flag indicating if the job should be run in the foreground.
    - **Output**: No return value. Launches the job.
    - The job is launched by forking a child process. The child process will execute the processes in the job. The parent process will wait for the child process to finish if the job is not run in the background.

4. **is_ImplementedCmd(char *cmd)**:
    - **Purpose**: Checks if a given command is implemented as a built-in command.
    - **Input**: A string representing the command.
    - **Output**: Returns 1 if the command is a built-in command, 0 otherwise.

5. **wait_for_job(Job *job)**:
    - **Purpose**: Waits for a job to finish.
    - **Input**: A pointer to a `Job` structure representing the job.
    - **Output**: No return value. Waits for the job to finish.
    - Waits for the job to finish by waiting for all processes in the job to finish. Waits for job with `WUNTRACED` flag, and updates the job status accordingly. If the job is stopped, the shell is put in the foreground and a message is printed.
  
6. **remove_job_from_job_list()**:
    - **Purpose**: Removes `DONE` jobs from the job list.

7. **put_job_in_foreground(Job *job, int cont)**:
    - **Purpose**: Puts a job in the foreground.
    - **Input**: 
      - `job`: A pointer to a `Job` structure representing the job.
      - `cont`: A flag indicating if the job should be continued.
    - **Output**: No return value. Puts the job in the foreground.
    - Puts the job in the foreground by setting the terminal process group ID to the job's process group ID. Waits for the job to finish if the job is stopped. Updates the job status and prints the job information if necessary. Sends a `SIGCONT` signal to the job's process group if the job should be continued.

8. **put_job_in_background(Job *job, int cont)**:
    - **Purpose**: Puts a job in the background.
    - **Input**: 
      - `job`: A pointer to a `Job` structure representing the job.
      - `cont`: A flag indicating if the job should be continued.
    - **Output**: No return value. Puts the job in the background.
    - Puts the job in the background by setting the job status to `RUNNING`. Prints the job information if necessary. Sends a `SIGCONT` signal to the job's process group if the job should be continued.

9.  **check_and_reset_job_num()**:
    - **Purpose**: Checks and resets the job number if necessary. If there are no background processes, the job number and recency are reset to 1.

### 4.5 Other Helper Functions
1. **free_tokens(Token *tokens)**:
   - **Purpose**: Frees memory allocated for tokens.

2. **free_process(Process *proc)**:
   - **Purpose**: Frees memory allocated for a process structure.

3. **free_job(Job *job)**:
   - **Purpose**: Frees memory allocated for a job structure.

4. **print_jobs()**:
   - **Purpose**: Prints information about all active jobs. Used for debugging purposes.

## 5. Built-in Commands Implementation & Design
&nbsp; &nbsp; The shell will support built-in commands such as `bg`, `fg`, `jobs`, `cd`, `pwd`, and `exit`. The built-in commands are implemented as functions inside `pa2.c`. The built-in commands are executed in the `launch_job` function same as any other commands.

### 5.1 `fg` Command (`builtin_fg`) && `bg` Command (`builtin_bg`)
- **Purpose**: Puts a job in the foreground or background.
- If no job number is provided, the function puts the most recent background job in the foreground or background.
- If a job number is provided, the function puts the specified job in the foreground/background by iterating through the list of background jobs and finding the job with the specified job number.
- If the job is not found, an error message is printed.
****
### 5.2 `jobs` Command (`builtin_jobs`)
- **Purpose**: Prints information about all active jobs.
- Iterates through the list of jobs and prints information about each job.

### 5.3 `cd` Command (`builtin_cd`)
- **Purpose**: Changes the current working directory.
- If no directory is provided, the function changes the current working directory to the home directory.
- If a directory is provided, the function changes the current working directory to the specified directory.

### 5.4 `pwd` Command (`builtin_pwd`)
- **Purpose**: Prints the current working directory.
- If the -P option is provided, prints the current physical directory.
- If the -L option is provided, prints the logical directory from the environment variable PWD.
- If an invalid option is provided, prints an error message.

### 5.5 `exit` Command (`builtin_exit`)
- **Purpose**: Exits the shell.
- Exits the shell with the given code


## 6. Self-Implemented Executables Implementation & Design
### 6.1 `head` Command (`pa2_head.c`)
- **Purpose**: Prints the first `n` lines of a file.

1. **Function: print_head(char *filename, int num_lines)**:
   - Prints the first `num_lines` lines of a file specified by `filename` or from standard input (`stdin`).

2. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Sets the number of lines to print based on the `-n` option or defaults to 10.
   - Prints a help message and exits if `-h` is provided.
   - Reads from standard input (`stdin`) if no filename is provided or the filename is `-`, otherwise reads from the specified file.
   - Invokes the `print_head` function with the appropriate arguments based on the command-line options and arguments.
   - Returns 0 to indicate successful execution.

### 6.2 `tail` Command (`pa2_tail.c`)
- **Purpose**: Prints the last `n` lines of a file.

1. **Function: print_tail(char *file_name, int num_lines)**:
   - Prints the last `num_lines` lines of a file specified by `file_name` or from standard input (`stdin`).
   - Opens the file or redirects to `stdin` if `file_name` is `-`.
   - Checks for errors while opening the file and if it's a directory.
   - Counts the total number of lines in the file.
   - Calculates the number of lines to skip based on the specified number of lines to print.
   - Moves the file pointer to the line to start printing from.
   - Prints the specified number of lines from the calculated starting point.
   - Frees memory allocated for line buffer and closes the file if it's not `stdin`.

2. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Sets the number of lines to print based on the `-n` option or defaults to 10.
   - Prints a help message and exits if `-h` is provided.
   - Reads from standard input (`stdin`) if no filename is provided or the filename is `-`, otherwise reads from the specified file.
   - Invokes the `print_tail` function with the appropriate arguments based on the command-line options and arguments.
   - Returns 0 to indicate successful execution.


### 6.3 `cat` Command (`pa2_cat.c`)
- **Purpose**: Concatenates and prints files.

1. **Function: print_file(const char *filename)**:
   - Prints the contents of a file specified by `filename` to standard output (`stdout`).
   - Opens the file or redirects to standard input (`stdin`) if `filename` is `-`.
   - Checks for errors while opening the file and if it's a directory.
   - Prints the contents of the file character by character until reaching the end of the file (`EOF`).
   - Closes the file if it's not `stdin`.

2. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Prints a help message and exits if `-h` is provided.
   - If no filename is provided or the filename is `-`, reads from standard input (`stdin`), otherwise reads from the specified file(s).
   - Invokes the `print_file` function with the appropriate arguments based on the command-line options and arguments.
   - Returns 0 to indicate successful execution.


### 6.4 `cp` Command (`pa2_cp.c`)
- **Purpose**: Allows users to copy the contents of one or more files to another file or directory.

1. **Function: copy_file(const char *src, const char *dst)**:
   - Opens the source file specified by `src` for reading and checks for errors.
   - Opens or creates the destination file specified by `dst` for writing and checks for errors.
   - Copies the contents of the source file to the destination file using a buffer.
   - Closes both files after copying.

2. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Prints a help message and exits if `-h` is provided.
   - Checks if there are enough arguments provided.
   - Determines if the last argument is a directory.
   - Copies each source file to the destination file or directory, handling directory paths appropriately.


### 6.5 `mv` Command (`pa2_mv.c`)
- **Purpose**: Allows users to move one or more files to another file or directory.

1. **Function: move_file(const char *src, const char *dst)**:
   - Renames the file specified by `src` to the name specified by `dst`.
   - Checks for errors during the renaming process and prints appropriate error messages.

2. **Function: is_dest_subdir(const char *src, const char *dst)**:
   - Determines if the destination directory specified by `dst` is a subdirectory of the source file specified by `src`.
   - Uses `realpath` to resolve the absolute paths of both source and destination.

3. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Prints a help message and exits if `-h` is provided.
   - Checks if there are enough arguments provided.
   - Determines if the last argument is a directory.
   - Loops through each source file and checks various conditions before attempting to move it.
   - Moves each source file to the destination file or directory, handling directory paths appropriately.


### 6.6 `rm` Command (`pa2_rm.c`)
- **Purpose**: Allows users to remove one or more files or directories.

1. **Function: main(int argc, char *argv[])**:
   - Parses command-line options using `getopt`.
   - Prints a help message and exits if `-h` is provided.
   - Checks if there are enough arguments provided. If not, it prints an error message and exits.
   - Loops through each file argument starting from index 1.
   - Tries to remove each file using the `unlink` function.
   - If an error occurs during the removal of a file, it prints an error message indicating the file name and the specific error encountered.
