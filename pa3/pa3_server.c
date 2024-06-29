#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <argon2.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// CONSTANTS ====================================================================

#define MAXLINE 2048
#define MEMORY_USAGE 512
#define SALT_SIZE 16
#define HASH_SIZE 32
#define HASH_PWD_SIZE 128
#define TIMEOUT_MILLISECONDS 100

// STRUCTS ======================================================================

typedef enum {
  TERMINATION,
  LOGIN,
  BOOK,
  CONFIRM_BOOKING,
  CANCEL_BOOKING,
  LOGOUT,
} Action;

// These pragmas are used to not pad the struct
#pragma pack(push, 1)
typedef struct Request {
  uint32_t user;
  uint32_t size;
  uint8_t action;
  uint8_t *data;
} Request;

typedef struct Response {
  uint32_t code;
  uint32_t size;
  uint8_t *data;
} Response;
#pragma pack(pop)

typedef struct User {
  uint32_t user_id;
  // int is_logged_in;
  struct User *next;
} User;

typedef struct ClientTask {
  int conn_fd;      // connection file descriptor
  uint32_t user_id; // user id of the client (if 0, not logged in)
  struct ClientTask *next;
} ClientTask;

typedef struct TaskQueue {
  ClientTask *head;
  ClientTask *tail;
  pthread_mutex_t task_lock;
  pthread_cond_t task_cond;
} TaskQueue;

// FUNCTION PROTOTYPES =========================================================
// client functions (helpers for main)
void *task_worker(void *arg);
void handle_client(ClientTask *session);
Response handle_action(Request request, uint32_t *user_id);

// receive and send functions
int recv_request(int fd, Request *request);
int send_response(int fd, Response response);

// login/password functions
void generate_salt(uint8_t *salt);
void hash_password(char *password, char *hashed_password);
int validate_password(char *password, char *hash_password);
int login(uint32_t entering_user, char *password);

// helper functions
void accept_connection();
int convert_data_to_int(uint8_t *data, size_t size);
char *convert_data_to_string(uint8_t *data, size_t size);
void handle_exit();
void handle_sigint(int sig);
void setup_signal_handler();
void setup_socket(in_port_t port);
void setup_signal_handler();

// debugging functions
void print_response(Response *response);
void print_request(Request *request);

// thread pool functions
void init_task_queue(TaskQueue *task_queue);
void enqueue_task(TaskQueue *task_queue, int conn_fd, uint32_t user_id);
ClientTask *dequeue_task(TaskQueue *task_queue);
int get_num_cores();

// GLOBAL VARIABLES =============================================================
int listen_fd = -1;                                             // listening socket
int seats[256] = {0};                                           // array to store the seat numbers (0 = empty, user_id = booked)
pthread_mutex_t seats_mutex[256] = {PTHREAD_MUTEX_INITIALIZER}; // mutex for each seat
pthread_mutex_t pwd_file_mutex = PTHREAD_MUTEX_INITIALIZER;     // mutex for password file
User *user_list_head = NULL;                                    // linked list of users
pthread_mutex_t user_list_mutex = PTHREAD_MUTEX_INITIALIZER;    // mutex for user list
TaskQueue task_queue;

// MAIN =========================================================================
int main(int argc, char *argv[]) {
  atexit(handle_exit);
  setup_signal_handler();

  // Check for port number
  if (argc < 2) {
    fprintf(stderr, "Received %d arguments. Please enter port number!\n", argc - 1);
    exit(1);
  }

  // get port number
  in_port_t port = (in_port_t)strtol(argv[1], NULL, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "invalid port number %s\n", argv[1]);
    exit(1);
  }

  // set up sockets for server
  setup_socket(port);

  // initialize task queue
  init_task_queue(&task_queue);

  // initialize thread pool
  const int THREAD_POOL_SIZE = get_num_cores();
  // const int THREAD_POOL_SIZE = 1; // for testing purposes
  pthread_t thread_pool[THREAD_POOL_SIZE];
  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    pthread_create(&thread_pool[i], NULL, task_worker, NULL);
  }

  while (1) {
    puts("Waiting for connection");

    // initialize user
    int conn_fd = -1;
    accept_connection(&conn_fd);           // Accept connection request from clients
    enqueue_task(&task_queue, conn_fd, 0); // add the client to the task queue
  }

  // join threads
  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    pthread_join(thread_pool[i], NULL);
  }

  close(listen_fd);
  listen_fd = -1;
  return 0;
}

// CLIENT FUNCTIONS ============================================================
void *task_worker(void *arg) {
  while (1) {
    ClientTask *session = dequeue_task(&task_queue);
    handle_client(session);
  }
  return NULL;
}

void handle_client(ClientTask *session) {
  while (1) {
    // Receive client action
    Request request;
    memset(&request, 0, sizeof(Request));

    struct pollfd fds;
    fds.fd = session->conn_fd;
    fds.events = POLLIN;

    int poll_result = poll(&fds, 1, TIMEOUT_MILLISECONDS); // 100 ms timeout
    if (poll_result == 0) {
      goto terminate;
    }

    if (recv_request(session->conn_fd, &request) == -1) {
      if (request.data != NULL)
        free(request.data);
      puts("Client disconnected.");
      close(session->conn_fd);
      break;
    }

    // Print request
    // print_request(&request);ㅋ

    // if the request is TERMINATION and the user is logged in, send a logout request first
    if (request.action == TERMINATION && session->user_id && request.user == 0 && convert_data_to_int(request.data, request.size) == 0){
      Request logout_request;
      memset(&logout_request, 0, sizeof(Request));
      logout_request.user = session->user_id;
      logout_request.size = 0;
      logout_request.action = LOGOUT;
      logout_request.data = NULL;

      Response logout_response = handle_action(logout_request, &(session->user_id));
      send_response(session->conn_fd, logout_response);

      // Free memory
      if (logout_response.data != NULL)
        free(logout_response.data);
    }

    // handle the action according to the request
    Response response;
    response = handle_action(request, &(session->user_id));

    // Send response
    send_response(session->conn_fd, response);

    // Free memory
    if (response.data != NULL)
      free(response.data);
    if (request.data != NULL)
      free(request.data);

  terminate:
    // in case of a successful TERMINATION
    if (response.code == 0 && request.action == TERMINATION) {
      enqueue_task(&task_queue, session->conn_fd, session->user_id);
      break;
    }
  }

  // close(session->conn_fd);
  free(session);
}

Response handle_action(Request request, uint32_t *user_id) {
  Response response;
  response.code = 0;
  response.size = 0;
  response.data = NULL;

  User *current_user = user_list_head;

  switch (request.action) {
  case TERMINATION:
    // printf("user %u: termination\n", request.user);
    // printf("data: %d\n", convert_data_to_int(request.data, request.size));

    // valid termination request
    if (convert_data_to_int(request.data, request.size) == 0 && request.user == 0) {
      puts("Terminating connection");
      break; // conn_fd closed in handle_client after the response is sent
    }

    // invalid termination request (not all fields are 0)
    puts("Invalid termination request");
    response.code = 1;

    break;
  // ==============================================================================
  case LOGIN:
    // printf("user %u: trying to login\n", request.user);
    
    // check if the user id is 0
    if(request.user == 0){
      response.code = 4;
      break;
    }

    // 1. if the user is active (logged into other client)
    pthread_mutex_lock(&user_list_mutex);
    current_user = user_list_head;
    while (current_user != NULL) {
      if (current_user->user_id == request.user && *user_id == 0) { // current_user->is_logged_in &&
        pthread_mutex_unlock(&user_list_mutex);
        response.code = 1;
        goto login_end; // exit the switch statement
      }
      current_user = current_user->next;
    }
    pthread_mutex_unlock(&user_list_mutex);

    // 2. if the client is active (already logged in)
    // printf("is_logged_in: %d\n", *is_logged_in);
    // printf("user_id: %u\n", *user_id);
    if (*user_id != 0) {
      response.code = 2;
      break;
    }

    // successful login
    if (login(request.user, (char *)request.data) > 0) {
      *user_id = request.user; // set user_id
      // printf("user_id set: %u\n", *user_id);

      // add user to the list of users
      User *new_user = (User *)malloc(sizeof(User));
      if (new_user) {
        new_user->user_id = request.user;
        // new_user->is_logged_in = 1;
        pthread_mutex_lock(&user_list_mutex);
        new_user->next = user_list_head;
        user_list_head = new_user;
        pthread_mutex_unlock(&user_list_mutex);
      }
    }
    // login failed
    else {
      response.code = 3;
    }

  login_end:
    break;
  // ==============================================================================
  case BOOK:
    int booking_seat = convert_data_to_int(request.data, request.size);
    // printf("user %u: trying to book seat %d\n", request.user, booking_seat);

    // 1: user is not logged in
    // printf("is_logged_in: %d\n", *is_logged_in);
    // printf("user_id: %u\n", *user_id);
    if (!*user_id || (*user_id != request.user)) {
      response.code = 1;
      break;
    }

    // 3: seat number is out of range
    if (booking_seat < 1 || booking_seat > 256) {
      response.code = 3;
      break;
    }

    // 2: invalid seat number (already booked)
    pthread_mutex_lock(&seats_mutex[booking_seat - 1]);
    // seat is already booked
    if (seats[booking_seat - 1]) {
      pthread_mutex_unlock(&seats_mutex[booking_seat - 1]);
      response.code = 2;
      break;
    }

    // seat is empty, successful booking
    seats[booking_seat - 1] = (int)request.user;
    response.size = sizeof(booking_seat);
    response.data = (uint8_t *)malloc(response.size);
    memcpy(response.data, &booking_seat, response.size);

    pthread_mutex_unlock(&seats_mutex[booking_seat - 1]);
    break;
  // ==============================================================================
  case CONFIRM_BOOKING:
    // printf("user %u: confirm booking\n", request.user);

    // 1. user is not logged in
    if (!*user_id || *user_id != request.user) {
      response.code = 1;
      break;
    }

    // if data = 0, check for all available seats
    if (convert_data_to_int(request.data, request.size) == 0) {
      char *available_seats = (char *)malloc(MAXLINE * sizeof(char)); // 2048 (to address commas, space, digit length)
      available_seats[0] = '\0';

      int i;
      for (i = 0; i < 256; i++) {
        pthread_mutex_lock(&seats_mutex[i]);
        if (seats[i] == 0) {
          sprintf(available_seats + strlen(available_seats), "%d, ", i + 1);
        }
        pthread_mutex_unlock(&seats_mutex[i]);
      }

      // remove the last comma and space
      if (strlen(available_seats) > 2)
        available_seats[strlen(available_seats) - 2] = '\0';

      // set response
      response.size = strlen(available_seats);
      response.data = (uint8_t *)available_seats;
    }

    // if request.size == 0, send all the booked seats by the user
    else if (request.size == 0){
      char *booked_seats = (char *)malloc(MAXLINE * sizeof(char)); // 2048 (to address commas, space, digit length)
      booked_seats[0] = '\0';

      int i;
      for (i = 0; i < 256; i++) {
        pthread_mutex_lock(&seats_mutex[i]);
        if (seats[i] == (int)request.user) {
          sprintf(booked_seats + strlen(booked_seats), "%d, ", i + 1);
        }
        pthread_mutex_unlock(&seats_mutex[i]);
      }

      // remove the last comma and space
      if (strlen(booked_seats) > 2)
        booked_seats[strlen(booked_seats) - 2] = '\0';

      // set response
      response.size = strlen(booked_seats);
      response.data = (uint8_t *)booked_seats;
    } else{
      response.code = 2;
    }
    break;
  case CANCEL_BOOKING:
    int cancel_seat = convert_data_to_int(request.data, request.size);
    // printf("user %u: cancel booking seat %d\n", request.user, cancel_seat);

    // 1: user is not logged in
    if (!*user_id || *user_id != request.user) {
      response.code = 1;
      break;
    }

    // 3: seat number is out of range
    if (cancel_seat < 1 || cancel_seat > 256) {
      response.code = 3;
      break;
    }

    // 2: user did not book the specified seat
    pthread_mutex_lock(&seats_mutex[cancel_seat - 1]);
    if (seats[cancel_seat - 1] != (int)request.user) {
      pthread_mutex_unlock(&seats_mutex[cancel_seat - 1]);
      response.code = 2;
      break;
    }

    // seat is empty, successful cancellation
    seats[cancel_seat - 1] = 0;
    response.size = sizeof(cancel_seat);
    response.data = (uint8_t *)malloc(response.size);
    memcpy(response.data, &cancel_seat, response.size);
    pthread_mutex_unlock(&seats_mutex[cancel_seat - 1]);

    break;
  // ==============================================================================
  case LOGOUT:
    // printf("user %u: logout\n", request.user);

    // 1: user is not logged in
    if (!*user_id || *user_id != request.user) {
      response.code = 1;
      break;
    }

    // flag to check if the user is found
    int user_found = 0;

    // remove user from the list of users
    pthread_mutex_lock(&user_list_mutex);
    current_user = user_list_head;
    User *prev_user = NULL;
    while (current_user) {
      if (current_user->user_id == request.user) {
        // current_user->is_logged_in = 0;
        if (prev_user) {
          prev_user->next = current_user->next;
        } else {
          user_list_head = current_user->next;
        }
        free(current_user);

        *user_id = 0; // reset user id

        user_found = 1;
        break;
      }
      prev_user = current_user;
      current_user = current_user->next;
    }
    pthread_mutex_unlock(&user_list_mutex);

    // if the user is not found (not logged in)
    if (!user_found) {
      response.code = 1;
    }
    break;
  // ==============================================================================
  default:
    printf("user %u: invalid action\n", request.user);
    break;
  }

  // print_response(&response);
  return response;
}

// RECEIVE AND SEND FUNCTIONS ===================================================
// receive request from client
int recv_request(int fd, Request *request) {
  uint8_t header[sizeof(request->user) + sizeof(request->size) + sizeof(request->action)];

  if (read(fd, header, sizeof(header)) <= 0)
    return -1;

  memcpy(&(request->user), header, sizeof(request->user));
  memcpy(&(request->size), header + sizeof(request->user), sizeof(request->size));
  memcpy(&(request->action), header + sizeof(request->user) + sizeof(request->size), sizeof(request->action));

  if (request->size == 0) {
    request->data = NULL;
    return 0;
  }

  request->data = (uint8_t *)malloc(request->size);
  if (read(fd, request->data, request->size) == 0) {
    free(request->data);
    return -1;
  }

  return 0;
}

// send response to client
int send_response(int fd, Response response) {
  size_t response_size = sizeof(response.code) + sizeof(response.size) + response.size;
  uint8_t *response_buffer = (uint8_t *)malloc(response_size);

  memcpy(response_buffer, &(response.code), sizeof(response.code));
  memcpy(response_buffer + sizeof(response.code), &(response.size), sizeof(response.size));

  if (response.size > 0)
    memcpy(response_buffer + sizeof(response.code) + sizeof(response.size), response.data, response.size);

  if (write(fd, response_buffer, response_size) == -1) {
    free(response_buffer);
    return -1;
  }

  free(response_buffer);
  return 0;
}

// LOGIN PASSWORD FUNCTIONS ===========================================================
void generate_salt(uint8_t *salt) {
  int fd = open("/dev/urandom", O_RDONLY);
  read(fd, salt, SALT_SIZE);
  close(fd);
}

void hash_password(char *password, char *hashed_password) {
  uint8_t salt[SALT_SIZE];
  generate_salt(salt);
  char hash[HASH_PWD_SIZE];
  argon2id_hash_encoded(2, MEMORY_USAGE, 1, password, strlen(password), salt, SALT_SIZE, HASH_SIZE, hash, HASH_PWD_SIZE);
  strcpy(hashed_password, hash);
}

int validate_password(char *password, char *hash_password) {
  if (argon2id_verify(hash_password, password, strlen(password)) == ARGON2_OK)
    return 1;
  else
    return 0;
}

int login(uint32_t entering_user, char *password) {
  pthread_mutex_lock(&pwd_file_mutex);
  FILE *file = fopen("/tmp/passwords.tsv", "a+");
  if (file == NULL) {
    perror("fopen");
    pthread_mutex_unlock(&pwd_file_mutex);
    return -1;
  }

  char *line = NULL;
  size_t len = 0;

  // search for user in file by iterating through each line
  while (getline(&line, &len, file) != -1) {
    // printf("line: %s\n", line);
    char *user_id = strtok(line, "\t");
    char *hashed_password = strtok(NULL, "\t");
    hashed_password[strlen(hashed_password) - 1] = '\0'; // remove newline character
    // printf("user_id: %s, hashed_password: %s\n", user_id, hashed_password);

    // if user_id or hashed_password is NULL, continue
    if (user_id == NULL || hashed_password == NULL)
      continue;

    // not the user we are looking for
    if (entering_user != strtoul(user_id, NULL, 10))
      continue;

    // if password is correct, return 1
    if (validate_password(password, hashed_password)) {
      fclose(file);
      pthread_mutex_unlock(&pwd_file_mutex);
      // printf("argon found user %d\n", entering_user);
      return 1;
    }
    // if password is incorrect, return 0
    else {
      fclose(file);
      pthread_mutex_unlock(&pwd_file_mutex);
      return -1;
    }
  }

  // if user not found, add them to the file and return 0
  printf("User %d not found. Adding to file\n", entering_user);
  if (ftell(file) == 0 || fseek(file, 0, SEEK_END) == 0) {
    char new_user_hash_pwd[HASH_PWD_SIZE];
    hash_password(password, new_user_hash_pwd);
    fprintf(file, "%d\t%s\n", entering_user, new_user_hash_pwd);
  }

  // clean up
  fclose(file);
  pthread_mutex_unlock(&pwd_file_mutex);

  if (line != NULL)
    free(line);

  return 1;
}

// SETUP FUNCTIONS ============================================================
// handles sigint signal
void handle_sigint(int sig) {
  if (sig == SIGINT)
    exit(0);
}

// handles exit for the listening socket
void handle_exit() {
  if (listen_fd != -1)
    close(listen_fd);
}

// sets up signal handler for sigint
void setup_signal_handler() {
  struct sigaction act;
  act.sa_handler = handle_sigint;
  sigaction(SIGINT, &act, NULL);
}

// sets up socket for server
void setup_socket(in_port_t port) {
  struct sockaddr_in sock_addr;

  // Create listen socket
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket");
    exit(1);
  }

  // Bind sockaddr (IP, etc.) to listen socket
  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port = htons(port);

  // Bind listen socket
  if (bind(listen_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
    perror("bind");
    exit(1);
  }

  // Listen to listen socket
  if (listen(listen_fd, 5) < 0) { // 5 is the maximum number of clients
    perror("listen");
    exit(1);
  }
}

// accepts connection from client
void accept_connection(int *conn_fd) {
  socklen_t conn_addr_len;
  struct sockaddr_in conn_addr;

  conn_addr_len = sizeof(conn_addr);
  if ((*conn_fd = accept(listen_fd, (struct sockaddr *)&conn_addr, &conn_addr_len)) < 0) {
    perror("accept");
    exit(1);
  }

  printf("Client connected: %s\n", inet_ntoa(conn_addr.sin_addr));
}

// HELPER FUNCTIONS ============================================================
// converts the data(little endian hex) from the client into int
int convert_data_to_int(uint8_t *data, size_t size) {
  if (size == 0)
    return -1;

  int result = 0;
  for (size_t i = 0; i < size; i++) {
    result |= data[i] << (i * 8);
  }
  return result;
}

// converts the data from the client into string
char *convert_data_to_string(uint8_t *data, size_t size) {
  if (size == 0)
    return NULL;

  char *result = malloc(size + 1);
  memcpy(result, data, size);
  result[size] = '\0';
  return result;
}

// for debugging purposes ========================================================
void print_response(Response *response) {
  printf("Code: %u, Size: %u, Data: ", response->code, response->size);
  for (size_t i = 0; i < response->size; i++) {
    printf("%02x ", response->data[i]);
  }
  printf("\n");
}

void print_request(Request *request) {
  printf("User: %u, Size: %u, Action: %u, Data: ", request->user, request->size, request->action);
  for (size_t i = 0; i < request->size; i++) {
    printf("%02x ", request->data[i]);
  }
  printf("\n");
}

// THREAD POOL FUNCTIONS ========================================================
void init_task_queue(TaskQueue *task_queue) {
  task_queue->head = NULL;
  task_queue->tail = NULL;
  pthread_mutex_init(&task_queue->task_lock, NULL);
  pthread_cond_init(&task_queue->task_cond, NULL);
}

void enqueue_task(TaskQueue *task_queue, int conn_fd, uint32_t user_id) {
  ClientTask *new_task = (ClientTask *)malloc(sizeof(ClientTask));
  new_task->conn_fd = conn_fd;
  new_task->user_id = user_id;
  new_task->next = NULL;

  pthread_mutex_lock(&task_queue->task_lock);
  if (task_queue->head == NULL) {
    task_queue->head = new_task;
    task_queue->tail = new_task;
  } else {
    task_queue->tail->next = new_task;
    task_queue->tail = new_task;
  }
  pthread_cond_signal(&task_queue->task_cond);
  pthread_mutex_unlock(&task_queue->task_lock);
}

ClientTask *dequeue_task(TaskQueue *task_queue) {
  pthread_mutex_lock(&task_queue->task_lock);
  while (task_queue->head == NULL) {
    pthread_cond_wait(&task_queue->task_cond, &task_queue->task_lock);
  }

  ClientTask *task = task_queue->head;
  task_queue->head = task->next;
  if (task_queue->head == NULL) {
    task_queue->tail = NULL;
  }
  pthread_mutex_unlock(&task_queue->task_lock);
  return task;
}

int get_num_cores() {
  cpu_set_t cpu_set;
  sched_getaffinity(0, sizeof(cpu_set), &cpu_set);
  return CPU_COUNT_S(sizeof(cpu_set), &cpu_set);
}