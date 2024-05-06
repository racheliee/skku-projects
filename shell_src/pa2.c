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
#define MAX_ARGS 100   // maximum number of arguments
#define MAX_TOKENS 200 // maximum number of tokens
#define MAX_PATH 4096  // maximum path length
#define MAX_JOBS 8192  // maximum number of jobs

// structs ===========================================================================
typedef enum {
    READY,
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
    int is_background; // if the job is backgrounded
    int is_ampersand;  // if the job is backgrounded with &
    int is_piped;      // for pipe
    int is_redirected; // for input and output redirection
    int is_append;     // for output redirection
    int recency;       // the most recent job has the highest recency
    char *input_file;
    char *output_file;
} Job;

// global variables ===================================================================
Job *first_job = NULL;
int job_num = 1; // job number for the next job
int shell_terminal;
pid_t shell_pgid;
const char *shell_name = "pa2_shell";
int recency = 1; // the most recent job has the highest recency
char *status[4] = {"Ready", "Running", "Stopped", "Done"};
char done_buffer[MAX_PATH]; // buffer for done job messages
char *root;                 // root directory

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
int is_ImplmentedCmd(char *cmd);
void exec_cmd(int argc, char *arg[]);

void remove_job_from_job_list();
void wait_for_job(Job *job);
void put_job_in_foreground(Job *job, int cont);
void put_job_in_background(Job *job, int cont);
void check_and_reset_job_num();
void launch_job(Job *job);
void evaluate_cmd(char *cmd);
void init_shell();
void sig_handler(int signo);

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
        printf("status: %s\n", status[cur_job->status]);
        printf("recent: %d\n", cur_job->recency);
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
    job->status = READY;
    job->pgid = 0;
    job->first_process = NULL;
    job->is_background = 0;
    job->is_piped = 0;
    job->is_redirected = 0;
    job->is_append = 0;
    job->is_ampersand = 0;
    job->recency = 0;
    job->next = NULL;
    job->input_file = NULL;
    job->output_file = NULL;

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
            cur_proc->status = READY;
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
            break;
        default:
            break;
        }
    }

    job->input_file = job->first_process->input_file;
    job->output_file = cur_proc->output_file;

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
        if (proc->args[i] != NULL)
            free(proc->args[i]);
    }
    if (proc->input_file != NULL) {
        free(proc->input_file);
    }
    if (proc->output_file != NULL) {
        free(proc->output_file);
    }
    free(proc);
    return;
}

void free_job(Job *job) {
    Process *cur_proc = job->first_process;
    Process *next_proc;
    while (cur_proc != NULL) {
        next_proc = cur_proc->next;
        free(cur_proc);
        cur_proc = next_proc;
    }

    if (job->input_file != NULL) {
        free(job->input_file);
    }
    if (job->output_file != NULL) {
        free(job->output_file);
    }

    free(job);
}

// built-in command functions ==================================================================
void builtin_fg(char *arg[]) {
    // if no job number is provided
    if (arg[1] == NULL) {
        Job *most_recent_job = NULL;
        int job_recency = 0;
        Job *job = first_job;

        // loop through the job list to find the most recent job
        while (job != NULL) {
            if (job->is_background && job->recency > job_recency) {
                most_recent_job = job;
                job_recency = job->recency;
            }
            job = job->next;
        }

        // send the most recent job to foreground
        if(most_recent_job != NULL) {
            // print the message
            for (int i = 0; i < most_recent_job->first_process->argc; i++) {
                printf("%s ", most_recent_job->first_process->args[i]);
            }
            printf("\n");
            // set the job to foreground
            put_job_in_foreground(most_recent_job, 1);
            return;
        }

        // if no job is found
        printf("%s: fg: current: no such job\n", shell_name);
    }
    // if job number is provided
    else {
        int job_number = atoi(arg[1] + 1); // +1 to skip the % sign

        // if the job number is invalid (string or less than 1)
        if (job_number < 1) {
            printf("%s: fg: %s: no such job\n", shell_name, arg[1]);
            return;
        }

        // find job with the given job number and send to foreground
        Job *j = first_job;
        while (j != NULL) {
            if (j->job_num == job_number && j->is_background) {
                for (int i = 0; i < j->first_process->argc; i++) {
                    printf("%s ", j->first_process->args[i]);
                }
                printf("\n");
                put_job_in_foreground(j, 1);
                return;
            }
            j = j->next;
        }
        printf("%s: fg: %d: no such job\n", shell_name, job_number);
    }
    return;
}

void builtin_bg(char *arg[]) {
    // no job number, set most recent job to background
    if (arg[1] == NULL) {
        Job *most_recent_job = NULL;
        int job_recency = 0;
        Job *job = first_job;

        while (job != NULL) {
            if (job->status == STOPPED && job->is_background && job->recency > job_recency) {
                most_recent_job = job;
                job_recency = job->recency;
            }
            job = job->next;
        }

        // send the most recent job to background
        if(most_recent_job != NULL) {
            // print the message
            printf("[%d] \t", most_recent_job->job_num);
            for (int i = 0; i < most_recent_job->first_process->argc; i++) {
                printf("%s ", most_recent_job->first_process->args[i]);
            }
            printf("&\n");
            // set the job to background
            put_job_in_background(most_recent_job, 1);
            return;
        }

        // if no job is found
        printf("%s: bg: current: no such job\n", shell_name);
    }
    // job number is given
    else {
        int job_number = atoi(arg[1] + 1); // +1 to skip the % sign
        Job *j = first_job;

        // if the job number is invalid
        if (job_number < 1) {
            printf("%s: bg: %s: no such job\n", shell_name, arg[1]);
            return;
        }

        while (j != NULL) {
            if (j->job_num == job_number && j->status == STOPPED) {
                if (j->is_ampersand) {
                    printf("%s: bg: %d: job already in background\n", shell_name, job_number);
                    return;
                }

                // print the message
                printf("[%d] \t", j->job_num);
                for (int i = 0; i < j->first_process->argc; i++) {
                    printf("%s ", j->first_process->args[i]);
                }
                printf("&\n");
                // set the job to background
                put_job_in_background(j, 1);
                return;
            }
            j = j->next;
        }

        printf("%s: bg: %d: no such job\n", shell_name, job_number);
    }
    return;
}

void builtin_jobs(char *arg[]) {
    Job *job_list[MAX_JOBS];
    int job_count = 0;
    Job *j = first_job->next; // skip the first job (jobs)

    while (j != NULL) {
        if (j->status != DONE) {
            job_list[job_count++] = j;
        }
        j = j->next;
    }

    for (int i = job_count - 1; i >= 0; i--) {
        Job *job = job_list[i];
        if (job->recency == recency - 1) {
            printf("[%d]+\t%s \t", job->job_num, status[job->status]);
        } else if (job->recency == recency - 2) {
            printf("[%d]-\t%s \t", job->job_num, status[job->status]);
        } else {
            printf("[%d]\t%s \t", job->job_num, status[job->status]);
        }

        Process *p = job->first_process;
        for (int j = 0; j < p->argc; j++) {
            printf("%s ", p->args[j]);
        }
        if (job->is_ampersand) {
            printf("&");
        }
        printf("\n");
    }

    return;
}

void builtin_pwd(char *arg[]) {
    // no option (print logical directory by default)
    if (arg[1] == NULL) {
        char cwd[MAX_PATH];
        if (getcwd(cwd, MAX_PATH) == NULL) {
            fprintf(stderr, "%s: pwd: %s\n", shell_name, strerror(errno));
            exit(1);
        } else {
            printf("%s\n", cwd);
        }
        return;
    }
    // -P option (print physical directory)
    if (strcmp(arg[1], "-P") == 0) {
        char cwd[MAX_PATH];
        if (getcwd(cwd, MAX_PATH) == NULL) {
            fprintf(stderr, "%s: pwd: %s\n", shell_name, strerror(errno));
            exit(1);
        } else {
            printf("%s\n", cwd);
        }
        return;
    }

    // -L option (print logical directory)
    if (strcmp(arg[1], "-L") == 0) {
        char *cwd = getenv("PWD");
        if (cwd == NULL) {
            fprintf(stderr, "%s: pwd: %s\n", shell_name, strerror(errno));
            exit(1);
        } else {
            printf("%s\n", cwd);
        }
        return;
    }

    // invalid option (begins with - but not -L or -P)
    if (arg[1][0] == '-') {
        fprintf(stderr, "%s: pwd: -%c: invalid option\n", shell_name, arg[1][0]);
        fprintf(stderr, "pwd: usage: pwd [-LP]\n");
        return;
    }
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
    if (arg[1] == NULL) {
        exit(0);
        return;
    }

    char *end;
    long exit_code = strtol(arg[1], &end, 10);

    // if there are more than 1 argument
    if (arg[2] != NULL) {
        fprintf(stderr, "%s: exit: too many arguments\n", shell_name);
        return;
    }

    // invalid argument, no digits parsed
    if (end == arg[1] || exit_code < 0) {
        fprintf(stderr, "%s: exit: %s: numeric argument required\n", shell_name, arg[1]);
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
    if (is_ImplmentedCmd(arg[0])) {
        // fixme: won't work if pa2 is not executed from bin
        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/pa2_%s", root, arg[0]);
        // printf("path: %s\n", path);
        execv(path, arg);
        perror("execv error");
        exit(1);
    } else {
        if (execvp(arg[0], arg) == -1) {
            fprintf(stderr, "%s: %s: %s\n", shell_name, arg[0], strerror(errno));
            exit(1);
        }
    }
}

// job control ========================================================================
void remove_job_from_job_list() {
    Job *prev_job = NULL;
    Job *cur_job = first_job;

    while (cur_job != NULL) {
        if (cur_job->status == DONE) {
            if (prev_job == NULL) {
                first_job = cur_job->next;
            } else {
                prev_job->next = cur_job->next;
            }
            Job *temp = cur_job->next;
            free_job(cur_job);
            cur_job = temp;
        } else {
            prev_job = cur_job;
            cur_job = cur_job->next;
        }
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
                perror("waitpid:");
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
                check_and_reset_job_num();
                job->job_num = job_num++;
            }
            // update the recency
            job->recency = recency++;

            // print the message
            printf("\n[%d]+\tStopped %d\n", job->job_num, job->pgid);

            return;
        } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
            job->status = DONE;
        }
    }

    remove_job_from_job_list();

    return;
}

void put_job_in_foreground(Job *job, int cont) {
    tcsetpgrp(shell_terminal, job->pgid);
    job->is_background = 0;

    if (cont) {
        if (kill(-job->pgid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
            tcsetpgrp(shell_terminal, shell_pgid);
            return;
        }
        job->status = RUNNING;
    }

    wait_for_job(job);

    tcsetpgrp(shell_terminal, shell_pgid);
}

void put_job_in_background(Job *job, int cont) {
    job->is_background = 1;
    job->status = RUNNING;

    // set job number if it's not set
    if (!job->job_num) {
        check_and_reset_job_num();
        job->job_num = job_num++;
    }
    // update the recency
    job->recency = recency++;

    if (cont) {
        if (kill(-job->pgid, SIGCONT) < 0) {
            perror("kill (SIGCONT)");
        }
    }
}

void check_and_reset_job_num() {
    int background_job_count = 0;

    Job *j = first_job;
    while (j != NULL) {
        if ((j->is_background && j->status != DONE) || j->status == STOPPED) {
            background_job_count++;
            // if there is more than 1 background job, break
            // 1 background job is the job that was just stopped or added
            if (background_job_count > 1)
                break;
        }
        j = j->next;
    }

    if (background_job_count < 2) {
        job_num = 1;
        recency = 1; // reset the recency if needed
    }

    return;
}

void launch_job(Job *job) {
    Process *p;
    pid_t pid;
    int pipe_fd[2];
    int in_fd = STDIN_FILENO;
    int out_fd;
    job->status = RUNNING;

    // check if the input and output files are the same. if so, return
    if (job->input_file != NULL && job->output_file != NULL && strcmp(job->input_file, job->output_file) == 0) {
        fprintf(stderr, "%s: -: input and output file are the same\n", job->first_process->args[0]);
        return;
    }

    // set input file for first process
    if (job->first_process->input_file != NULL) {
        in_fd = open(job->first_process->input_file, O_RDONLY);
        // return if the input file cannot be opened
        if (in_fd < 0) {
            fprintf(stderr, "%s: -: %s: %s\n", job->first_process->args[0], job->first_process->input_file, strerror(errno));
            return;
        }
    }

    // add the job to the job list
    job->next = first_job;
    first_job = job;

    // these commands have to be here bc process does not need to be forked and redirection/pipe is not supported
    if (strcmp(job->first_process->args[0], "fg") == 0) {
        job->status = DONE;
        builtin_fg(job->first_process->args);
        return;
    } else if (strcmp(job->first_process->args[0], "bg") == 0) {
        job->status = DONE;
        builtin_bg(job->first_process->args);
        return;
    } else if (strcmp(job->first_process->args[0], "exit") == 0) {
        job->status = DONE;
        builtin_exit(job->first_process->args);
        return;
    } else if (strcmp(job->first_process->args[0], "cd") == 0) {
        job->status = DONE;
        builtin_cd(job->first_process->args);
        return;
    }

    // print_jobs();

    for (p = job->first_process; p != NULL; p = p->next) {
        if (p->is_next_pipe) {
            if (pipe(pipe_fd) < 0) {
                fprintf(stderr, "%s: pipe: %s\n", shell_name, strerror(errno));
                return;
            }
            out_fd = pipe_fd[1];

            // if output file is set, open the file
            if (p->output_file != NULL) {
                out_fd = open(p->output_file, O_WRONLY | O_CREAT | (p->is_append ? O_APPEND : O_TRUNC), 0644);
                // return if the output file cannot be opened
                if (out_fd < 0) {
                    fprintf(stderr, "%s: -: %s: %s\n", job->first_process->args[0], job->first_process->input_file, strerror(errno));
                    return;
                }
            }
        }
        // set output file
        else {
            out_fd = (p->output_file != NULL) ? open(p->output_file, O_WRONLY | O_CREAT | (p->is_append ? O_APPEND : O_TRUNC), 0644) : 1;
            // return if the output file cannot be opened
            if (out_fd < 0) {
                fprintf(stderr, "%s: -: %s: %s\n", job->first_process->args[0], job->first_process->input_file, strerror(errno));
                return;
            }
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

                // if the job is not background, set the terminal to the job's process group
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
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (out_fd != STDOUT_FILENO) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            // execute the command
            if (strcmp(p->args[0], "jobs") == 0) {
                builtin_jobs(p->args);
                exit(0);
            } else if (strcmp(p->args[0], "pwd") == 0) {
                builtin_pwd(p->args);
                exit(0);
            } else if (strcmp(p->args[0], "exit") == 0) {
                builtin_exit(p->args);
                return;
            } else {
                exec_cmd(p->argc, p->args);
            }
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
        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            close(out_fd);
        }
        // set the input file descriptor to the read end of the pipe
        // and close the write end of the pipe in the parent
        if (p->is_next_pipe) {
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        } else {
            in_fd = 0;
        }
        in_fd = pipe_fd[0];
    }

    // wait for the job to finish if the shell is not interactive
    if (!isatty(shell_terminal)) {
        wait_for_job(job);
    } else if (!job->is_background) {
        put_job_in_foreground(job, 0);
    } else {
        put_job_in_background(job, 0);
        printf("[%d]+\t %d\n", job->job_num, job->pgid);
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

void sig_handler(int signum) {
    int status;
    pid_t pid;

    // reap all terminated or stopped child processes
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        Job *j = first_job;
        int found = 0;
        while (j != NULL) {
            Process *p = j->first_process;
            while (p != NULL) {
                if (p->pid == pid) {
                    found = 1;
                }
                p = p->next;
            }
            if (found)
                break;
            j = j->next;
        }

        if (j != NULL) {
            if (WIFSTOPPED(status)) {
                j->status = STOPPED;
                j->is_background = 1;
            } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
                j->status = DONE;

                if (j->is_background) {
                    // if a background process ends it should be printed the next time we're in the shell
                    int curr_len = strlen(done_buffer);
                    char arg_buffer[MAX_PATH];
                    int arg_index = 0;
                    for (int i = 0; i < j->first_process->argc; i++) {
                        for (int k = 0; k < strlen(j->first_process->args[i]); k++) {
                            arg_buffer[arg_index++] = j->first_process->args[i][k];
                        }
                        arg_buffer[arg_index++] = ' ';
                    }

                    int len = snprintf(done_buffer + curr_len, MAX_PATH - curr_len, "[%d]\tDone \t%s", j->job_num, arg_buffer);
                    if (len < 0 || len >= MAX_PATH - curr_len) {
                        fprintf(stderr, "%s: sig_handler: done_buffer: buffer overflow - too many done jobs to report.\n", shell_name);
                        exit(1);
                    }
                }
            }
        }
    }
}

// main ===============================================================================
int main() {
    root = getcwd(NULL, 0); // get the root directory
    init_shell();           // initialize the shell

    signal(SIGCHLD, sig_handler);

    while (1) {
        char *cmd;
        char cwd[MAX_PATH];

        if (getcwd(cwd, MAX_PATH) == NULL) {
            perror("getcwd error");
            exit(1);
        }

        // create prompt
        char prompt[MAX_PATH];
        strcpy(prompt, "pa2_shell: ");
        strcat(prompt, cwd);
        strcat(prompt, " $ ");

        if ((cmd = readline(prompt)) == NULL) {
            break;
        }

        add_history(cmd);
        evaluate_cmd(cmd);
        // print the message if there is a done background job
        if (strcmp(done_buffer, "") != 0) {
            printf("%s\n", done_buffer);
            strcpy(done_buffer, ""); // reset the buffer
        }

        free(cmd);
    }

    // free all jobs
    while (first_job != NULL) {
        Job *temp = first_job->next;
        free_job(first_job);
        first_job = temp;
    }

    return 0;
}