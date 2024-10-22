// You're free to change any part of this code.
// You can add more arguments if needed to existing functions.
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 80
#define MAX_USERNAME 20
#define LISTEN_BACKLOG 5

#define SAFELY_RUN(call, exit_code) \
  if ((call) < 0) {                 \
    perror(#call);                  \
    exit(exit_code);                \
  }

int listenfd;

enum MessageType {
  NEW_USER,
  USER_LEFT,
  USER_MESSAGE,
};

typedef struct {
  enum MessageType type;
  char message[MAXLINE];
  char username[MAX_USERNAME];
} message;

enum ExitCode {
  EXIT_CODE_SUCCESS,
  EXIT_CODE_SOCKET_CREATION_FAILURE,
  EXIT_CODE_SELECT_FAILURE,
  EXIT_CODE_WRITE_FAILURE,
  EXIT_CODE_READ_FAILURE,
  EXIT_CODE_INVALID_ARGUMENTS,
  EXIT_CODE_INVALID_PORT_NUMBER,
  EXIT_CODE_ACCEPT_FAILURE,
  EXIT_CODE_BIND_FAILURE,
  EXIT_CODE_LISTEN_FAILURE,
};

/* Helper functions */
void exit_handler(void);
void sigint_handler(int sig);
long get_port(int argc, char *argv[]);
int create_listening_socket(long port);

/* Functions you might have to modify */

// You might not need to modify this
// Depending on your implementation
void handle_new_connection(struct pollfd *fds, int *nfds) {
  int conn_fd;
  struct sockaddr_in conn_address;
  int conn_address_len = sizeof(conn_address);

  SAFELY_RUN(conn_fd = accept(listenfd, (struct sockaddr *)&conn_address,
                              (socklen_t *)&conn_address_len),
             EXIT_CODE_ACCEPT_FAILURE)

  fds[*nfds].fd = conn_fd;
  fds[*nfds].events = POLLIN;
  (*nfds)++;
}

// TODO: Should send the message to every client
void handle_client_data(int i,
                        struct pollfd *fds,
                        int *nfds,
                        size_t *n_clients) {
  message msg;
  memset(&msg, 0, sizeof(msg));
  ssize_t n;

  if ((n = read(fds[i].fd, (void *)&msg, sizeof(msg))) > 0) {
    switch (msg.type) {
    case NEW_USER:
      (*n_clients)++;
      if (*n_clients == 1) {
        printf("%s has joined the chat. %zu active user.\n", msg.username,
               (*n_clients));
      } else {
        printf("%s has joined the chat. %zu active users.\n", msg.username,
               (*n_clients));
      }

      break;
    case USER_LEFT:
      (*n_clients)--;
      if (*n_clients == 1) {
        printf("%s has left the chat. %zu active user.\n", msg.username,
               (*n_clients));
      } else {
        printf("%s has left the chat. %zu active users.\n", msg.username,
               (*n_clients));
      }
      break;
    case USER_MESSAGE:
      printf("got %zd bytes from %s.\n", strnlen(msg.message, MAXLINE),
             msg.username);
      break;
    }

    // TODO: Send data to each server
    for (int j = 0; j < *nfds; j++) {
      if (fds[j].fd != listenfd && j != i) {
        SAFELY_RUN(write(fds[j].fd, (void *)&msg, sizeof(msg)),
                   EXIT_CODE_WRITE_FAILURE);
      }
    }

    if(msg.type == USER_LEFT){
      close(fds[i].fd);
      for(int j = i; j < *nfds - 1; j++){
        fds[j] = fds[j+1];
      }
      (*nfds)--;
    }

  } else if (n == 0 || errno == ECONNRESET) {
    // Client terminated, so the server does not need
    // to monitor the associated socket anymore
    message left_msg;
    left_msg.type = USER_LEFT;
    strncpy(left_msg.username, msg.username, MAX_USERNAME);
    (*n_clients)--;
    if (*n_clients == 1) {
      printf("%s has left the chat. %zu active user.\n", msg.username,
             (*n_clients));
    } else {
      printf("%s has left the chat. %zu active users.\n", msg.username,
             (*n_clients));
    }

    for (int j = 0; j < *nfds; j++) {
      if (fds[j].fd != listenfd && j != i) {
        SAFELY_RUN(write(fds[j].fd, (void *)&left_msg, sizeof(left_msg)),
                   EXIT_CODE_WRITE_FAILURE);
      }
    }

    close(fds[i].fd);
    for (int j = i; j < *nfds - 1; j++) {
      fds[j] = fds[j + 1];
    }
    (*nfds)--;
  } else {
    perror("read");
    exit(EXIT_CODE_READ_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  long port = get_port(argc, argv);
  signal(SIGINT, sigint_handler);

  listenfd = create_listening_socket(port);

  struct pollfd fds[FD_SETSIZE];
  int nfds = 1;
  int timeout_msecs = -1; // Block indefinitely until an event occurs
  int ret;

  fds[0] = (struct pollfd){.fd = listenfd, .events = POLLIN};
  size_t n_clients = 0; // Add this line before the while loop

  while (1) {
    ret = poll(fds, nfds, timeout_msecs);

    if (ret > 0) {
      for (int i = 0; i < nfds; i++) {
        if (fds[i].revents & POLLIN) {
          if (fds[i].fd == listenfd) {
            handle_new_connection(fds, &nfds);
          } else {
            handle_client_data(i, fds, &nfds, &n_clients); // Modify this line
          }
        }
      }
    } else if (ret < 0) {
      // Handle error
      return EXIT_CODE_SELECT_FAILURE;
    }
  }

  return 0;
}

// Helper functions
// You can remove these functions if you'd like
// I just added these functions to lessen the errors.
void exit_handler(void) {
  printf("Server has terminated!\n");
  close(listenfd);
}

void sigint_handler(int sig) {
  if (sig == SIGINT) {
    exit(EXIT_CODE_SUCCESS);
  }
}

long get_port(int argc, char *argv[]) {
  char *endptr;

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(EXIT_CODE_INVALID_ARGUMENTS);
  }

  long port = strtol(argv[1], &endptr, 10);

  if (*endptr != '\0') {
    printf("Invalid port number.\n");
    exit(EXIT_CODE_INVALID_PORT_NUMBER);
  }

  return port;
}

int create_listening_socket(long port) {
  struct sockaddr_in socket_address;

  SAFELY_RUN((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)),
             EXIT_CODE_SOCKET_CREATION_FAILURE)
  atexit(exit_handler);

  memset((char *)&socket_address, 0, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  socket_address.sin_port = htons(port);

  SAFELY_RUN(
      bind(listenfd, (struct sockaddr *)&socket_address, sizeof(socket_address)),
      EXIT_CODE_BIND_FAILURE)

  SAFELY_RUN(listen(listenfd, LISTEN_BACKLOG), EXIT_CODE_LISTEN_FAILURE)

  return listenfd;
}