# System Programming Lab - PA2 

## Objective
&nbsp; &nbsp; The goal of this assignment is to implement a mini-shell program that supports basic shell commands. The shell should be able to execute commands, handle signals, job control, support I/O redirection, and support background processing. 

&nbsp; &nbsp; The first goal is to implement 6 executables found in GNU Core Utilities: `head`, `tail`, `cat`, `cp`, `mv`, and `rm`. The second goal was is to implement 6 built-in commands: `bg`, `fg`, `jobs`, `cd`, `pwd`, and `exit`. 

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
