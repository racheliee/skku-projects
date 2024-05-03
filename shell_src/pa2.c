#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 100
#define MAX_TOKENS 200

// structs ===========================================================================
typedef enum {
    READY,
    RUNNING,
    STOPPED,
    DONE,
    ZOMBIE,
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
    int job_id;
    pid_t pgid;
    Status status;
    struct JOB *next;
    struct PROCESS *first_process;
    int is_background;
    int is_piped;
    int is_redirected;
    int is_append;
} Job;

// global variables ===================================================================
Job *first_job = NULL;
int job_id = 0;
int shell_terminal;
pid_t shell_pgid;

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
        printf("job: %d\n", cur_job->job_id);
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
    job->job_id = 0;
    job->pgid = 0;
    job->status = READY;
    job->first_process = NULL;
    job->is_background = 0;
    job->is_piped = 0;
    job->is_redirected = 0;
    job->is_append = 0;
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
            for(int j = 0; j < MAX_ARGS; j++){
                cur_proc->args[j] = NULL;
            }
            cur_proc->args[0] = strdup(tokens[i].data);
            cur_proc->pid = 0;
            cur_proc->status = READY;
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
            job->job_id = job_id;
            job_id++;
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
    if (proc->next != NULL){
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
    if(job->next != NULL){
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
            if (job->status == STOPPED) {
                put_job_in_background(job, 1);
                // comeback: this process should come to the first job?
                return;
            } else if (job->status == READY) { // [ ] 사실상 stopped만 존재하지 않나?
                job->is_background = 1;
                launch_job(job);
                return;
            } else if (job->status == RUNNING) {
                put_job_in_background(job, 0); // no need to continue since it's already running
                return;
            }
            job = job->next;
        }

        fprintf(stderr, "pa2_shell: bg: current: no such job\n");
    } else {
        int job_number;

        if ((job_number = atoi(arg[1] + 1)) == 0) {
            fprintf(stderr, "pa2_shell bg: %s: no such job\n", arg[1]);
            return;
        }
    }
}

void builtin_fg(char *arg[]) {
    // todo: implement this
}

void builtin_jobs(char *arg[]) {
}

void builtin_pwd(char *arg[]) {

}

void builtin_cd(char *arg[]) {
}

void builtin_exit(char *arg[]) {
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
    if(strcmp(cmd, "head") == 0)
        return 1;
    if(strcmp(cmd, "tail") == 0)
        return 1;
    if(strcmp(cmd, "cat") == 0)
        return 1;
    if(strcmp(cmd, "mv") == 0)
        return 1;
    if(strcmp(cmd, "cp") == 0)
        return 1;
    if(strcmp(cmd, "rm") == 0)
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
        char path[1024];
        sprintf(path, "bin/pa2_%s", arg[0]);
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



void wait_for_job(Job *job) {
    int status;

    while (waitpid(-job->pgid, &status, WUNTRACED) > 0) {
        if (WIFSTOPPED(status)) {
            job->status = STOPPED;
        } else if (WIFEXITED(status)) {
            job->status = DONE;
            // remove_job_from_job_list(job);
        } else if (WIFSIGNALED(status)) { // comeback: this is weird lol
            job->status = ZOMBIE;
            // remove_job_from_job_list(job);
        }
    }
}

void put_job_in_foreground(Job *job, int cont) {
    tcsetpgrp(shell_terminal, job->pgid);
    job->is_background = 0;
    job->job_id = 0;

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

    // assign job_id
    if (job->job_id == 0) {
        job->job_id = job_id;
        job_id++;
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

        if ((cmd = readline("$ ")) == NULL) {
            break;
        }

        add_history(cmd);
        evaluate_cmd(cmd);
        free(cmd);
    }

    // free all jobs
    // Job *cur_job = first_job;
    // Job *next_job;
    // while (cur_job != NULL) {
    //     remove_job_from_job_list(cur_job);
    // }

    return 0;
}