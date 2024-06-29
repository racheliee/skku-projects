#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#define MAX_INSTR 1000
#define BUF_SIZE 27

void exec(char *cmd) {
    unsigned int pid;
    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        execlp("sh", "sh", "-c", cmd, (char *)NULL);
        exit(0);
    }

    while(waitpid(pid, NULL, WNOHANG) == 0){
        continue;
    }

    return;
}

int main(void) {
    setlogmask(LOG_MASK(LOG_EMERG));
    openlog("cron", LOG_PID, LOG_LPR);

    int fd;
    char buf[512];
    int fd0, fd1, fd2;
    time_t t;
    struct tm *tm;

    fd = open("/tmp/crontab", O_RDONLY);
    if (errno == ENOENT) {
        perror("crontab does not exist");
        exit(1);
    }

    switch (fork()) {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(0);
    }

    if (setsid() < 0) {
        perror("Failed to create a new session");
        exit(2);
    }

    if (chdir("/") < 0) {
        perror("Failed to change directory to root directory");
        exit(3);
    }

    umask(0);

    for (int std_fd = 0; std_fd < 3; std_fd++)
        close(std_fd);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = open("/dev/null", O_RDWR);
    fd2 = open("/dev/null", O_RDWR);

    char instr_list[MAX_INSTR][3][100];
    int instr_count = 0;

    // read crontab file
    ssize_t bytes_read = 0;

    while ((bytes_read = read(fd, buf, BUF_SIZE)) > 0) {
        buf[bytes_read] = '\0';
        char *line = NULL;
        char *pos = buf;
        int rewind_offset = 0;

        // for each line, read the argv and save it to instr_list
        while ((line = strtok_r(pos, "\n", &pos)) != NULL) {
            int i = 0;
            rewind_offset = strlen(line) + 1;

            char *token = NULL;
            char *save_ptr;
            token = strtok_r(line, " ", &save_ptr);
            while (token != NULL && i < 3) {
                strcpy(instr_list[instr_count][i++], token);
                token = strtok_r(NULL, " ", &save_ptr);
            }
            instr_count++;
        }

        // move the file pointer to the start of the line if the line is not fully read
        if (bytes_read == BUF_SIZE) {
            lseek(fd, -rewind_offset, SEEK_CUR);
            instr_count--;
        }
    }

    while (1) {

        for (int i = 0; i < instr_count; i++) {
            t = time(NULL);
            tm = localtime(&t);

            // get the minute and hour (default -1 if not *)
            long min = -1, hour = -1;

            if (strcmp(instr_list[i][0], "*") != 0) {
                min = strtol(instr_list[i][0], NULL, 10); // convert string to long
            }
            if (strcmp(instr_list[i][1], "*") != 0) {
                hour = strtol(instr_list[i][1], NULL, 10);
            }

            if (min == -1) {      // minute is *
                if (hour == -1) { // both * *
                    exec(instr_list[i][2]);
                } else {
                    if (tm->tm_hour == hour) {
                        exec(instr_list[i][2]);
                    }
                }
            } else { // minute is not *
                if (hour == -1) {
                    if (tm->tm_min == min) {
                        exec(instr_list[i][2]);

                    }
                } else {
                    if (tm->tm_min == min && tm->tm_hour == hour) {
                        exec(instr_list[i][2]);
                    }
                }
            }
        }

        t = time(NULL);
        tm = localtime(&t);
        sleep(60 - tm->tm_sec % 60);

        // reap all child zombie processes
        while (waitpid(-1, NULL, WNOHANG) > 0) {
            continue;
        }
    }

    closelog();
    close(fd);
    close(fd0);
    close(fd1);
    close(fd2);
    return 0;
}