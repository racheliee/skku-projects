#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// macros ===========================================================================
#define MAX_ARGS 100
#define MAX_TOKENS 200
#define MAX_PATH 4096

// structs ===========================================================================
typedef enum {
    RUNNING,
    STOPPED,
    DONE,
} Status;

typedef enum {
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_PIPE,            // |
    TOKEN_REDIRECT_IN,     // <
    TOKEN_REDIRECT_OUT,    // >
    TOKEN_REDIRECT_APPEND, // >>
    TOKEN_BACKGROUND,      // &
    TOKEN_END,
} TokenType;

typedef struct {
    TokenType type;
    char *data; // actual content of the token
} Token;

typedef struct PROCESS {
    pid_t pid;
    Status status;
    struct PROCESS *next;
    char *args[MAX_ARGS];
    int argc;
    char *input_file;
    char *output_file;
    int is_append;
    int is_next_pipe;
} Process;

typedef struct JOB {
    int job_num;
    pid_t pgid;
    Status status;
    struct JOB *next;
    struct PROCESS *first_process;
    int is_background;
    int is_ampersand;
    int is_piped;
    int is_redirected;
    int is_append;
} Job;

// global variables ===================================================================
Job *first_job = NULL;
int job_num = 1;
int shell_terminal;
pid_t shell_pgid;
const char *shell_name = "pa2_shell";
int recent_job_num[2] = {0, 0};

// function prototypes ===============================================================
int is_special_char(char c);
int is_pipe(Token token);
int is_redirection(Token token);
void lex(char *cmd, Token *tokens);

void print_jobs();
Job *parse(Token *tokens);

void free_tokens(Token *tokens);
void free_process(Process *proc);
void free_job(Job *job);

void builtin_bg(char *arg[]);
void builtin_fg(char *arg[]);
void builtin_jobs(char *arg[]);
void builtin_pwd(char *arg[]);
void builtin_cd(char *arg[]);
void builtin_exit(char *arg[]);
int is_BuiltInCmd(char *cmd);
int is_ImplmentedCmd(char *cmd);
void exec_cmd(int argc, char *arg[]);

void remove_job_from_job_list(Job *job);
void check_background_job_status();
void wait_for_job(Job *job);
void put_job_in_foreground(Job *job, int cont);
void put_job_in_background(Job *job, int cont);
void launch_job(Job *job);
void evaluate_cmd(char *cmd);
void init_shell();

// lexing & its helpers ===============================================================
int is_special_char(char c) {
    return c == '<' || c == '>' || c == '|' || c == '\'' || c == '"' ||
           c == '\0' || isspace(c) || c == '&';
}

int is_pipe(Token token) {
    return token.type == TOKEN_PIPE;
}

int is_redirection(Token token) {
    return token.type == TOKEN_REDIRECT_IN || token.type == TOKEN_REDIRECT_OUT ||
           token.type == TOKEN_REDIRECT_APPEND;
}

void lex(char *cmd, Token *tokens) {
    int token_i = 0; // token index

    for (const char *curr_char = cmd; *curr_char != '\0'; curr_char++) {
        if (isspace(*curr_char)) {
            continue;
        }

        switch (*curr_char) {
        case '|':
            tokens[token_i++] = (Token){TOKEN_PIPE, "|"};
            break;
        case '<':
            tokens[token_i++] = (Token){TOKEN_REDIRECT_IN, "<"};
            break;
        case '>':
            if (*(curr_char + 1) == '>') {
                tokens[token_i++] = (Token){TOKEN_REDIRECT_APPEND, ">>"};
                curr_char++;
            } else {
                tokens[token_i++] = (Token){TOKEN_REDIRECT_OUT, ">"};
            }
            break;
        case '&':
            tokens[token_i++] = (Token){TOKEN_BACKGROUND, "&"};
            break;
        case '\'':
        case '\"': {
            // not required for PA2, but for safety; takes care of quotes
            char quote = *curr_char;
            const char *start = ++curr_char;
            while (*curr_char != quote && *curr_char != '\0') {
                curr_char++;
            }
            tokens[token_i++] = (Token){
                .type = TOKEN_ARGUMENT,
                .data = strndup(start, curr_char - start),
            };
        } break;
        default: {
            const char *cmd_start = curr_char;

            TokenType type = (is_pipe(tokens[token_i - 1]) || token_i == 0) ? TOKEN_COMMAND : TOKEN_ARGUMENT;

            while (!is_special_char(*curr_char)) {
                curr_char++;
            }

            tokens[token_i++] = (Token){type, strndup(cmd_start, curr_char - cmd_start)};
            curr_char--;
            break;
        }
        }
    }

    tokens[token_i] = (Token){TOKEN_END, NULL};

    return;
}

// parsing & its helpers ==============================================================
void print_jobs() {
    Job *cur_job = first_job;
    while (cur_job != NULL) {
        Process *cur_proc = cur_job->first_process;
        printf("job: %d\n", cur_job->job_num);
        while (cur_proc != NULL) {
            printf("command: %s\n", cur_proc->args[0]);
            for (int i = 1; i < cur_proc->argc; i++) {
                printf("arg %d: %s\n", i, cur_proc->args[i]);
            }
            if (cur_proc->input_file != NULL) {
                printf("input file: %s\n", cur_proc->input_file);
            }
            if (cur_proc->output_file != NULL) {
                printf("output file: %s\n", cur_proc->output_file);
            }
            cur_proc = cur_proc->next;
        }
        cur_job = cur_job->next;
        printf("\n");
    }
}

Job *parse(Token *tokens) {
    if (tokens[0].type == TOKEN_END) {
        return NULL;
    }

    Job *job = (Job *)malloc(sizeof(Job));
    job->job_num = 0;
    job->pgid = 0;
    job->first_process = NULL;
    job->is_background = 0;
    job->is_piped = 0;
    job->is_redirected = 0;
    job->is_append = 0;
    job->is_ampersand = 0;
    job->next = NULL;

    Process *cur_proc = NULL;

    for (int i = 0; tokens[i].type != TOKEN_END; i++) {
        switch (tokens[i].type) {
        case TOKEN_COMMAND:
            if (job->first_process == NULL) {
                job->first_process = (Process *)malloc(sizeof(Process));
                cur_proc = job->first_process;
            } else {
                cur_proc->next = (Process *)malloc(sizeof(Process));
                cur_proc = cur_proc->next;
            }
            // initialize the process
            for (int j = 0; j < MAX_ARGS; j++) {
                cur_proc->args[j] = NULL;
            }
            cur_proc->args[0] = strdup(tokens[i].data);
            cur_proc->pid = 0;
            cur_proc->argc = 1; // since we have the command
            cur_proc->input_file = NULL;
            cur_proc->output_file = NULL;
            cur_proc->is_append = 0;
            cur_proc->is_next_pipe = 0;
            cur_proc->next = NULL;

            break;
        case TOKEN_ARGUMENT:
            cur_proc->args[cur_proc->argc++] = strdup(tokens[i].data);
            break;
        case TOKEN_PIPE:
            job->is_piped = 1;
            cur_proc->is_next_pipe = 1;
            break;
        case TOKEN_REDIRECT_IN:
            job->is_redirected = 1;
            job->first_process->input_file = strdup(tokens[++i].data);
            break;
        case TOKEN_REDIRECT_OUT:
            job->is_redirected = 1;
            cur_proc->output_file = strdup(tokens[++i].data);
            break;
        case TOKEN_REDIRECT_APPEND:
            job->is_redirected = 1;
            cur_proc->output_file = strdup(tokens[++i].data);
            cur_proc->is_append = 1;
            break;
        case TOKEN_BACKGROUND:
            job->is_background = 1;
            job->is_ampersand = 1;
            job->job_num = job_num;
            job_num++;
            break;
        default:
            break;
        }
    }

    return job;
}

// free functions =====================================================================
void free_tokens(Token *tokens) {
    for (int i = 0; tokens[i].type != TOKEN_END; i++) {
        if (tokens[i].type == TOKEN_COMMAND || tokens[i].type == TOKEN_ARGUMENT) {
            free(tokens[i].data);
        }
    }
}

void free_process(Process *proc) {
    for (int i = 0; i < proc->argc; i++) {
        free(proc->args[i]);
    }
    if (proc->input_file != NULL) {
        free(proc->input_file);
    }
    if (proc->output_file != NULL) {
        free(proc->output_file);
    }
    if (proc->next != NULL) {
        free_process(proc->next);
    }
    free(proc);
    return;
}

void free_job(Job *job) {
    Process *cur_proc = job->first_process;
    Process *next_proc;
    while (cur_proc != NULL) {
        next_proc = cur_proc->next;
        free_process(cur_proc);
        cur_proc = next_proc;
    }
    if (job->next != NULL) {
        free_job(job->next);
    }
    free(job);
}

// built-in command functions ==================================================================
void builtin_bg(char *arg[]) {
    // job number format: %<job_number>
    // no job number, set most recent job to background
    if (arg[1] == NULL) {
        Job *job = first_job;
        while (job->next != NULL) {
            // fixme: job->job_num == recent_job_num[0]
            if (job->status == STOPPED && job->is_background) {
                put_job_in_background(job, 1);
                // comeback: this process should come to the first job?
                return;
            }
            job = job->next;
        }

        printf("%s: bg: current: no such job\n", shell_name);
    }
    // job number is given
    else {
        // atoi is safe to use because 0 is not a valid job number
        int job_number = atoi(arg[1] + 1); // +1 to skip the % sign
        Job *j = first_job;

        // if the job number is invalid
        if (job_number < 1) {
            printf("%s: bg: %s: no such job\n", shell_name, arg[1]);
            return;
        }

        while (j != NULL) {
            if (j->job_num == job_number && j->status == STOPPED && !j->is_background) {
                put_job_in_background(j, 1);
                return;
            }
            j = j->next;
        }

        printf("%s: bg: %d: no such job\n", shell_name, job_number);
    }
    return;
}

void builtin_fg(char *arg[]) {
    // if no job number is provided
    if (arg[1] == NULL) {
        Job *job = first_job;
        while (job != NULL) {
            // fixme: job->job_num == recent_job_num[0]
            if (job->status == STOPPED && !job->is_background) {
                put_job_in_foreground(job, 1);
                return;
            }
            job = job->next;
        }
        return;
    }
    // if job number is provided
    else {
        // atoi is safe to use because 0 is not a valid job number
        int job_number = atoi(arg[1] + 1); // +1 to skip the % sign

        // if the job number is invalid (string or less than 1)
        if (job_number < 1) {
            printf("%s: fg: %s: no such job\n", shell_name, arg[1]);
            return;
        }

        // find job with the given job number and send to foreground
        Job *j = first_job;
        while (j != NULL) {
            if (j->job_num == job_number && j->status == STOPPED && j->is_background) {
                put_job_in_foreground(j, 1);
                return;
            }
            j = j->next;
        }
        printf("%s: fg: %d: no such job\n", shell_name, job_number);
    }
    return;
}

// fixme: printing order is wrong
void builtin_jobs(char *arg[]) {
    Job *j = first_job;
    char *status[2] = {"Running", "Stopped"};

    while (j != NULL) {
        if (j->status != DONE) {
            // most recent job
            if (j->job_num == recent_job_num[0]) {
                printf("[%d]+ %s ", j->job_num, status[j->status]);
                for (int i = 0; j->first_process->args[i] != NULL; i++)
                    printf("%s ", j->first_process->args[i]);
            }
            // second most recent job
            else if (j->job_num == recent_job_num[1]) {
                printf("[%d]- %s ", j->job_num, status[j->status]);
                for (int i = 0; j->first_process->args[i] != NULL; i++)
                    printf("%s ", j->first_process->args[i]);
            }
            // other jobs
            else {
                printf("[%d] %s ", j->job_num, status[j->status]);
                for (int i = 0; j->first_process->args[i] != NULL; i++)
                    printf("%s ", j->first_process->args[i]);
            }
            // todo: come back and fix this bash prints out & irregularly
            if (j->is_ampersand) {
                printf("&");
            }
            printf("\n");
        }
    }
}

void builtin_pwd(char *arg[]) {
    // -P option (print physical directory)
    if (strcmp(arg[1], "-P") == 0) {
        char cwd[MAX_PATH];
        if (getcwd(cwd, MAX_PATH) == NULL) {
            fprintf(stderr, "%s: pwd: %s\n", shell_name, strerror(errno));
            exit(1);
        } else {
            printf("%s\n", cwd);
        }
    }
    // invalid option (begins with - but not -L)
    else if (arg[1][0] == '-' && strcmp(arg[1], "-L")) {
        fprintf(stderr, "%s: pwd: -%c: invalid option\n", shell_name, arg[1][0]);
        fprintf(stderr, "pwd: usage: pwd [-LP]\n");
    }
    // -L options or other (print logical directory by default)
    else {
        char *cwd = getenv("PWD");
        if (cwd == NULL) {
            fprintf(stderr, "%s: pwd: %s\n", shell_name, strerror(errno));
            exit(1);
        } else {
            printf("%s\n", cwd);
        }
    }

    return;
}

void builtin_cd(char *arg[]) {
    // if no argument is passed, change to home directory
    if (arg[1] == NULL) {
        if (chdir(getenv("HOME")) == -1)
            fprintf(stderr, "%s: cd: %s: No such file or directory\n", shell_name, getenv("HOME"));
        return;
    }
    // if argument is passed, change to the directory
    if (chdir(arg[1]) == -1) {
        switch (errno) {
        case ENOENT:
            fprintf(stderr, "%s: cd: %s: No such file or directory\n", shell_name, arg[1]);
            break;
        case ENOTDIR:
            fprintf(stderr, "%s: cd: %s: Not a directory\n", shell_name, arg[1]);
            break;
        case EACCES:
            fprintf(stderr, "%s: cd: %s: Permission denied\n", shell_name, arg[1]);
            break;
        default:
            fprintf(stderr, "%s: cd: %s: %s\n", shell_name, arg[1], strerror(errno));
        }
        return;
    }
}

void builtin_exit(char *arg[]) {
    char *end;
    long exit_code = strtol(arg[1], &end, 10);

    // invalid argument, no digits parsed
    if (end == arg[1] || exit_code < 0) {
        fprintf(stderr, "%s: exit: %s: numeric argument required\n", shell_name, arg[1]);
        exit(2);
        return;
    }

    // if there are more than 1 argument
    if (arg[2] != NULL) {
        fprintf(stderr, "%s: exit: too many arguments\n", shell_name);
        exit(2);
        return;
    }

    if (exit_code > 255) {
        exit(255);
        return;
    }

    exit(exit_code);
    return;
}

int is_BuiltInCmd(char *cmd) {
    if (strcmp(cmd, "bg") == 0)
        return 1;
    if (strcmp(cmd, "fg") == 0)
        return 2;
    if (strcmp(cmd, "jobs") == 0)
        return 3;
    if (strcmp(cmd, "pwd") == 0)
        return 4;
    if (strcmp(cmd, "cd") == 0)
        return 5;
    if (strcmp(cmd, "exit") == 0)
        return 6;
    return 0;
}

int is_ImplmentedCmd(char *cmd) {
    if (strcmp(cmd, "head") == 0)
        return 1;
    if (strcmp(cmd, "tail") == 0)
        return 1;
    if (strcmp(cmd, "cat") == 0)
        return 1;
    if (strcmp(cmd, "mv") == 0)
        return 1;
    if (strcmp(cmd, "cp") == 0)
        return 1;
    if (strcmp(cmd, "rm") == 0)
        return 1;
    return 0;
}

void exec_cmd(int argc, char *arg[]) {
    // todo: implement this
    if (is_BuiltInCmd(arg[0])) {
        switch (is_BuiltInCmd(arg[0])) {
        case 1:
            builtin_bg(arg);
            break;
        case 2:
            builtin_fg(arg);
            break;
        case 3:
            builtin_jobs(arg);
            break;
        case 4:
            builtin_pwd(arg);
            break;
        case 5:
            builtin_cd(arg);
            break;
        case 6:
            builtin_exit(arg);
            break;
        default:
            break;
        }
    } else if (is_ImplmentedCmd(arg[0])) {
        // fixme: won't work if we move to different directories
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "bin/pa2_%s", arg[0]);
        execv(path, arg);
        perror("execv error");
        exit(1);
    } else {
        execvp(arg[0], arg);
        perror("execvp error");
        exit(1);
    }
}

// job control ========================================================================
void remove_job_from_job_list(Job *job) {
    Job *prev_job = NULL;
    Job *cur_job = first_job;
    while (cur_job != NULL) {
        if (cur_job == job) {
            if (prev_job == NULL) {
                first_job = cur_job->next;
            } else {
                prev_job->next = cur_job->next;
            }
            free_job(cur_job);
            break;
        }
        prev_job = cur_job;
        cur_job = cur_job->next;
    }

    return;
}

void check_background_job_status() {
    Job *j = first_job;
    while (j != NULL) {
        Process *p = j->first_process;
        while (p != NULL) {
            int status;
            pid_t wait_res = waitpid(p->pid, &status, WNOHANG);

            if (wait_res == -1) {
                perror("waitpid error");
                exit(1);
            } else if (wait_res == 0) {
                // process still running
            } else {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    p->status = DONE;
                    j->status = DONE;
                } else if (WIFSTOPPED(status)) {
                    p->status = STOPPED;
                    j->status = STOPPED;
                    return; //[ ] check again
                }
            }

            p = p->next;
        }
        Job *next = j->next;
        if (j->status == DONE) {
            remove_job_from_job_list(j);
        }
        j = next;
    }

    return;
}

void wait_for_job(Job *job) {
    int status;
    int pid_ret;

    while ((pid_ret = waitpid(-job->pgid, &status, WUNTRACED)) > 0) {
        if (pid_ret == -1) {
            if (errno == ECHILD) { // no child processes
                job->status = DONE;
                break;
            } else { // other errors
                perror("waitpid error");
                exit(1);
            }
        }

        if (WIFSTOPPED(status)) {
            job->status = STOPPED;
            job->is_background = 1;

            // set the terminal back to the shell and print the message
            tcsetpgrp(shell_terminal, shell_pgid);

            // print the message (+ since it's the most recent job)
            // add job number if it's not set
            if (!job->job_num) {
                job->job_num = job_num;
                job_num++;
            }
            printf("\n[%d]+ Stopped %d\n", job->job_num, job->pgid);

            // todo: move the job to the most recent job
            recent_job_num[1] = recent_job_num[0];
            recent_job_num[0] = job->job_num;

            return;
        } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
            job->status = DONE;
        }
    }

    remove_job_from_job_list(job);

    return;
}

void put_job_in_foreground(Job *job, int cont) {
    tcsetpgrp(shell_terminal, job->pgid);
    job->is_background = 0;
    job->job_num = 0;

    if (cont) {
        if (kill(-job->pgid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }
    }

    wait_for_job(job);

    tcsetpgrp(shell_terminal, shell_pgid);
}

void put_job_in_background(Job *job, int cont) {
    job->is_background = 1;

    // assign job_num
    if (job->job_num == 0) {
        job->job_num = job_num;
        job_num++;
    }

    if (cont) {
        if (kill(-job->pgid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }
    }
}

void launch_job(Job *job) {
    Process *p;
    pid_t pid;
    int pipe_fd[2];
    int in_fd = 0, out_fd = 1;
    job->status = RUNNING;

    for (p = job->first_process; p; p = p->next) {
        if (p->next != NULL && p->is_next_pipe) {
            if (pipe(pipe_fd) < 0) {
                perror("pipe error");
                exit(1);
            }
            out_fd = pipe_fd[1];
        } else { // set output file
            // todo: redirection error 처리
            out_fd = (p->output_file != NULL) ? open(p->output_file, O_WRONLY | O_CREAT | (p->is_append ? O_APPEND : O_TRUNC), 0644) : 1;
        }

        switch (pid = fork()) {
        case 0: // child process --> launch the process
            pid_t child_pid;
            if (isatty(shell_terminal)) { // set the child process group
                child_pid = getpid();
                if (!job->pgid) {
                    job->pgid = child_pid;
                }
                setpgid(child_pid, job->pgid);

                if (!job->is_background) {
                    tcsetpgrp(shell_terminal, job->pgid);
                }

                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL);
            }

            // set input and output file descriptors
            if (in_fd != 0) {
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (out_fd != 1) {
                dup2(out_fd, 1);
                close(out_fd);
            }

            // execute the command
            exec_cmd(p->argc, p->args);
            // execvp(p->args[0], p->args);
            // perror("execvp error");
            // remove_job_from_job_list(job); // [ ] check if its okay to remove the job here
            // exit(1);
        case -1:
            perror("fork error");
            exit(1);
        default: // parent process --> continue to the next process
            p->pid = pid;

            if (isatty(shell_terminal)) {
                if (!job->pgid) {
                    job->pgid = pid;
                }
                setpgid(pid, job->pgid);
            }
        }

        // close the pipes
        if (in_fd != 0) {
            close(in_fd);
        }
        if (out_fd != 1) {
            close(out_fd);
        }
        in_fd = pipe_fd[0];
    }

    if (!isatty(shell_terminal)) {
        wait_for_job(job);
    } else {
        if (!job->is_background) {
            put_job_in_foreground(job, 0);
        } else {
            put_job_in_background(job, 0);
        }
    }
}

void evaluate_cmd(char *cmd) {
    Token tokens[MAX_TOKENS];
    lex(cmd, tokens);
    Job *job = parse(tokens);
    if (job == NULL) {
        free_tokens(tokens);
        return;
    }
    job->next = first_job;
    first_job = job;
    // print_jobs();

    launch_job(job);
    free_tokens(tokens);
}

// shell initializations ===============================================================
void init_shell() {
    shell_terminal = STDIN_FILENO;

    if (isatty(shell_terminal)) {
        // loop until in the foreground
        while (tcgetpgrp(shell_terminal) != (getpgrp())) {
            kill(-getpgrp(), SIGTTIN);
        }

        // ignore interactive and job-control signals
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);

        // shell process group
        shell_pgid = getpid();
        if (setpgid(shell_pgid, shell_pgid) < 0) {
            perror("setpgid: shell was not able to set its own process group");
            exit(1);
        }

        // grab control of the terminal
        tcsetpgrp(shell_terminal, shell_pgid);
    }
}

// main ===============================================================================
int main() {
    init_shell();

    while (1) {
        char *cmd;
        char cwd[MAX_PATH];

        if (getcwd(cwd, MAX_PATH) == NULL) {
            perror("getcwd error");
            exit(1);
        } else {
            printf("%s: %s ", shell_name, cwd);
        }

        if ((cmd = readline("$ ")) == NULL) {
            break;
        }

        add_history(cmd);
        evaluate_cmd(cmd);
        check_background_job_status();
        free(cmd);
    }

    // free all jobs
    Job *cur_job = first_job;
    while (cur_job != NULL && cur_job->next != NULL) {
        remove_job_from_job_list(cur_job);
        cur_job = cur_job->next;
    }

    return 0;
}