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

int main(void) {
  unsigned int pid;
  time_t t;
  struct tm* tm;
  int fd;
  char* argv[3];
  char buf[512];
  int fd0, fd1, fd2;

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

  t = time(NULL);
  tm = localtime(&t);

  // You can change the code however you want including here or above.

  // 1. Possi
  while (1) {
    buf[0] = '\0';

    // insert your code

    // ##  hints  ##

    // strtok_r();
    // pid = fork();
    // execlp("sh", "sh", "-c", argv[2], (char*) NULL);
    // execl("/bin/sh", "/bin/sh", "-c", argv[2], (char*) NULL);

    t = time(NULL);
    tm = localtime(&t);

    sleep(60 - tm->tm_sec % 60);
  }

  /**
   *  2nd possible method
   *  Instead of one global while loop, you can go through
   *  each line of the crontab file and create a new process for each line.
   */

  // 2. Instead of one global while loop
  //

  return 0;
}