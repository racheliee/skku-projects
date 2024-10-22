#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ACK_TYPE 20
#define ERASE_CURRENT_LINE "\33[2K\r"

typedef struct {
  long msgtype;  // hint: ???
  int sender_id;
  char text[512];
} msgbuf;

typedef struct {
  long msgtype;  // hint: ACK_TYPE + ???
  char timestamp[512];
} msgbuf_ack;

void get_input(char** buf, size_t* size, char* unformatted_prompt, ...);
int get_id(char* text);
void sigint_handler(int sig);
void setup_handler();

int main() {
  setup_handler();
  int pid;
  int user_id = get_id("User ID: ");
  int receiver_id = get_id("Receiver ID: ");

  // TODO: create key and message queue
  key_t key = ftok(".", 'a');
  if(key == -1){
    perror("ftok error");
    exit(1);
  }
  int qid = msgget(key, IPC_CREAT | 0666);
  if(qid == -1){
    perror("msgget error");
    exit(1);
  }
  // ???

  switch (pid = fork()) {
    case -1:
      perror("fork");
      exit(1);
    case 0:
      // The child is the receiver
      while (1) {
        msgbuf buf;

        // TODO: Receive a message with IPC_NOWAIT
        if (msgrcv(qid, (void*)&buf, sizeof(buf), user_id, IPC_NOWAIT) != -1) {
          msgbuf_ack ack;
          ack.msgtype = ACK_TYPE + receiver_id;

          printf(ERASE_CURRENT_LINE "User %d:\t%s", buf.sender_id,
                 buf.text);  // This line erases the current line and prints the
                             // received message
          printf("User %d:\t", user_id);
          fflush(stdout);

          // TODO: Send an ack message (current timestamp) to the sender
          time_t current_time;
          time(&current_time);
          strftime(ack.timestamp, sizeof(ack.timestamp), "%Y-%m-%d %H:%M:%S",
                   localtime(&current_time));
          msgsnd(qid, (void*)&ack, sizeof(ack), 0);
          
        }

        // TODO: Receive ack message using IPC_NOWAIT and store the messeg in
        // `read_time`
        msgbuf_ack read_time;
        if (msgrcv(qid, (void*)&read_time, sizeof(read_time), ACK_TYPE + user_id, IPC_NOWAIT) != -1) {
          // NOTE: The printf statement below clears the current line and then
          // sends a message This printf only works properly when the sender and
          // receiver are running at the same itme Otherwise, it will clear the
          // input from the user's perspective, but not the actual stdin

          printf(ERASE_CURRENT_LINE "User %d read message at %s\n", receiver_id,
                 read_time.timestamp);
          printf("User %d:\t", user_id);
          fflush(stdout);
        }
      }
      break;
    default:
      // The parent is the sender
      while (1) {
        /** TODO:
         * 1. Get regular message from stdin (implemented)
         * 2. If the message is "quit", exit both parent and child (use
         * SIGINT)
         * 3. Send normal message
         */

        msgbuf buf;
        buf.msgtype = receiver_id;
        char* line = NULL;
        size_t len = 0;
        get_input(&line, &len, "User %d: ", user_id);


        if (strcmp(line, "quit\n") == 0) {
          kill(pid, SIGINT);
          waitpid(pid, NULL, 0);
          free(line);
          break;
        }

        // Your code
        buf.sender_id = user_id;
        strncpy(buf.text, line, sizeof(buf.text));
        msgsnd(qid, (void*)&buf, sizeof(buf), 0);

        free(line);
      }
      break;
  }

  return 0;
}

// Helper functions
void get_input(char** buf, size_t* size, char* unformatted_prompt, ...) {
  va_list args;
  va_start(args, unformatted_prompt);
  char* prompt;
  if (vasprintf(&prompt, unformatted_prompt, args) == -1) {
    perror("vasprintf");
    exit(1);
  }
  va_end(args);

  fputs(prompt, stdout);

  if (getline(buf, size, stdin) == -1) {
    if (errno == 0) {
      exit(0);
    }
    perror("getline");
    exit(1);
  }
}

int get_id(char* text) {
  char* input = NULL;
  size_t len;

  get_input(&input, &len, text);

  int id = (int)strtol(input, NULL, 10);
  free(input);
  return id;
}

void sigint_handler(int sig) {
  if (sig == SIGINT) {
    exit(0);
  }
}

void setup_handler() {
  struct sigaction action;
  action.sa_handler = sigint_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGINT, &action, NULL);
}
