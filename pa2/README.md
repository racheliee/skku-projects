# System Programming Lab PA2: Mini-Shell

## Objective
The goal of this assignment is to implement a mini-shell program that supports basic shell commands. The shell should be able to execute commands, handle signals, job control, support I/O redirection, and support background processing.   

The first goal is to implement 6 executables found in GNU Core Utilities: `head`, `tail`, `cat`, `cp`, `mv`, and `rm`. The second goal was is to implement 6 built-in commands: `bg`, `fg`, `jobs`, `cd`, `pwd`, and `exit`. 

## How to compile and run
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

## Key Features

### Functionalities
- **Command Execution**:
  - Execute system commands using `execvp`.
  - Handle I/O redirection and background processing.
- **Built-in Commands**:
  - **bg**: Move a job to the background.
  - **fg**: Move a job to the foreground.
  - **jobs**: List all background jobs.
  - **cd**: Change the current directory.
  - **pwd**: Print the current directory.
  - **exit**: Exit the shell.
- **Self-Implemented Executables**:
  - **head**: Print the first `n` lines of a file.
  - **tail**: Print the last `n` lines of a file.
  - **cat**: Concatenate and print files.
  - **cp**: Copy files.
  - **mv**: Move files.
  - **rm**: Remove files.

### Classes
- **Job Struct**: Manage job details, including process group ID, job number, status, and associated processes.
- **Process Struct**: Manage process details, including process ID, status, arguments, and I/O files.
- **Token Struct**: Store tokenized input commands and arguments.
- **Enums**:
  - **Status**: Define job or process status (Running, Stopped, Done).
  - **TokenType**: Define the type of token (Command, Argument, Redirection).

### Program Flow
- **Shell Initialization**: Set up the shell environment and signal handlers.
- **Main Loop**: Continuously prompt the user for input and execute commands.
- **Input Parsing**: Tokenize input commands and arguments, handle I/O redirection, and background processing.
- **Command Execution**: Execute built-in commands, self-implemented executables, or system commands using `execvp`.
- **Job Control**: Manage jobs using a linked list, handle signals to reap zombie processes, and manage foreground/background jobs.
