#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 80

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

// FUNCTION PROTOTYPES ==============================================================
void handle_action(Request *request);
void print_error_msg(Request *request, Response *response);
int send_request(int fd, Request *request);
int recv_response(int fd, Response *response);
void setup_conn_fd(char *host, in_port_t port);
void handle_sigint(int sig);
void handle_exit();
void setup_signal_handler();
int convert_data_to_int(uint8_t *data, size_t size);
char *convert_data_to_string(uint8_t *data, size_t size);
void print_response(Response *response);
void print_request(Request *request);


// GLOBAL VARIABLES ================================================================
int conn_fd = -1;  // Connection file descriptor
int logged_in = 0; // 0: not logged in, user id: logged in

// MAIN FUNCTION ====================================================================
int main(int argc, char *argv[]) {
  atexit(handle_exit);
  setup_signal_handler();

  // check for port number
  if (argc < 3) {
    fprintf(stderr, "Received %d arguments. Please enter host address and port number!\n", argc - 1);
    exit(1);
  }

  // get port number
  in_port_t port = (in_port_t)strtol(argv[2], NULL, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "invalid port number %s\n", argv[2]);
    exit(1);
  }

  // setup connection file descriptor
  setup_conn_fd(argv[1], port);

  char *request_line;
  while ((request_line = readline("Enter request >> "))) {
    if (strlen(request_line) == 0) {
      continue;
    }

    add_history(request_line);

    // parse request
    char *endptr;
    uint32_t user_id = (uint32_t)strtoul(request_line, &endptr, 10);
    uint8_t action = (uint8_t)strtoul(endptr, &endptr, 10);

    Request request = {.user = user_id, .size = 0, .action = action, .data = NULL};

    // get data field
    char *data = strtok(endptr, " ");

    // convert data to string (login), int (otherwise)
    if(data == NULL){
      request.size = 0;
      request.data = NULL;
    }else if(request.action == LOGIN){
      request.size = strlen(data) + 1;
      request.data = (uint8_t *)malloc(request.size);
      strcpy((char*)request.data, data);
    }else{ 
      unsigned long data_int = strtoul(data, NULL, 10);
      request.size = sizeof(data_int);
      request.data = (uint8_t *)malloc(request.size);
      memcpy(request.data, &data_int, request.size);
    }

    // print_request(&request);

    handle_action(&request); // handle the request
  }

  return 0;
}

// ACTION FUNCTIONS ===============================================================

void handle_action(Request *request) {
  Response response;
  switch (request->action) {
  case TERMINATION:
    // if user is logged in and data field is valid, logout first and then terminate the connection
    if (logged_in && convert_data_to_int(request->data, request->size) == 0) {
      // send logout request
      Request logout_request = {.user = request->user, .size = 0, .action = LOGOUT, .data = NULL};
      send_request(conn_fd, &logout_request);

      // if logout is successful, terminate the connection
      if (recv_response(conn_fd, &response) == 0) {
        logged_in = 0;

        // send termination request and terminate the connection
        send_request(conn_fd, request);
        if (recv_response(conn_fd, &response) == 0) {
          close(conn_fd);
          conn_fd = -1;
          printf("Connection Terminated\n");
          exit(0);
        }
      }
    } 
    // if the user is not logged in or the data is invalid
    else {
      send_request(conn_fd, request);

      if (recv_response(conn_fd, &response) == 0) {
        if (response.code == 0) {
          close(conn_fd);
          conn_fd = -1;
          printf("Connection Terminated\n");
          exit(0);
        } else {
          print_error_msg(request, &response);
        }
      }
    }
    break;

  // ============================================================================== 
  case LOGIN:
    send_request(conn_fd, request);

    if (recv_response(conn_fd, &response) == 0) {
      // print_response(&response);
      if (response.code == 0) {
        logged_in = 1;
        printf("Login Successful\n");
      } else {
        print_error_msg(request, &response);
      }
    }
    break;
  // ==============================================================================
  case BOOK:
    send_request(conn_fd, request);

    if(recv_response(conn_fd, &response) == 0){
      if(response.code == 0){
        printf("Booked seat %d\n", convert_data_to_int(response.data, response.size));
      }else{
        print_error_msg(request, &response);
      }
    }else{
      printf("Failed to book seat\n");
    }
    break;
  // ==============================================================================
  case CONFIRM_BOOKING:
    send_request(conn_fd, request);

    if(recv_response(conn_fd, &response) == 0){
      if(response.code == 0){
        // if request data field is empty, return seats booked by the user
        if(request->size == 0){
          // todo: print booked seats
        }
        // if request data field is 0, server returns all available seats 
        // todo: print available seats

        printf("Confirmed booking for seat %d\n", convert_data_to_int(response.data, response.size));
      }else{
        print_error_msg(request, &response);
      }
    }
    break;
  // ==============================================================================
  case CANCEL_BOOKING:
    send_request(conn_fd, request);

    if(recv_response(conn_fd, &response) == 0){
      if(response.code == 0){
        printf("Cancelled booking for seat %d\n", convert_data_to_int(response.data, response.size));
      }else{
        print_error_msg(request, &response);
      }
    }
    break;
  // ==============================================================================
  case LOGOUT:
    send_request(conn_fd, request);

    if(recv_response(conn_fd, &response) == 0){
      if(response.code == 0){
        logged_in = 0;
        printf("Logout Successful\n");
      }else{
        print_error_msg(request, &response);
      }
    }
    break;
  // ==============================================================================
  default:
    send_request(conn_fd, request);
    recv_response(conn_fd, &response);
    print_error_msg(request, &response);
    break;
  }
}

void print_error_msg(Request *request, Response *response) {
  switch (request->action) {
  case TERMINATION:
    if (response->code == 1)
      printf("Failed to disconnect as arguments are invalid\n");
    break;

  case LOGIN: //fixme: error codes
    if (response->code == 1)
      printf("Failed to login as arguments are invalid\n");
    else if (response->code == 2)
      printf("Failed to login as user is already logged in\n");
    else if (response->code == 3)
      printf("Failed to login as user does not exist\n");
    break;

  case BOOK:
    if (response->code == 1)
      printf("Failed to book as user is not logged in\n");
    else if (response->code == 2)
      printf("Failed to book as seat is unavailable\n");
    else if (response->code == 3)
      printf("Failed to book as seat number is out of range\n");
    break;

  case CONFIRM_BOOKING:
    if (response->code == 1)
      printf("Failed to confirm booking as user is not logged in\n");
    break;

  case CANCEL_BOOKING:
    if (response->code == 1)
      printf("Failed to cancel booking as user is not logged in\n");
    else if (response->code == 2)
      printf("Failed to cancel booking as user did not book the specified seat\n");
    else if (response->code == 3)
      printf("Failed to cancel booking as seat number is out of range\n");
    break;

  case LOGOUT:
    if (response->code == 1)
      printf("Failed to logout as user is not logged in\n");
    break;
  
  default:
    printf("Action %u is unknown\n", request->action); 
    break;
  }
}

// RECEIVE AND SEND FUNCTIONS =====================================================
int send_request(int fd, Request *request) {
  size_t request_size = sizeof(request->user) + sizeof(request->action) + sizeof(request->size) + request->size;
  uint8_t* request_buffer = (uint8_t*)malloc(request_size);

  memcpy(request_buffer, &request->user, sizeof(request->user));
  memcpy(request_buffer + sizeof(request->user), &request->size, sizeof(request->size));
  memcpy(request_buffer + sizeof(request->user) + sizeof(request->size), &request->action, sizeof(request->action));

  if (request->size > 0)
    memcpy(request_buffer + sizeof(request->user) + sizeof(request->size) + sizeof(request->action), request->data, request->size);

  if (write(fd, request_buffer, request_size) < 0){
    free(request_buffer);
    return -1;
  }

  free(request_buffer);
  return 0;
}

int recv_response(int fd, Response* response) {
  uint8_t header[sizeof(response->code) + sizeof(response->size)];
  if(read(fd, header, sizeof(header)) != sizeof(header))
    return -1;
  
  memcpy(&response->code, header, sizeof(response->code));
  memcpy(&response->size, header + sizeof(response->code), sizeof(response->size));

  if(response->size == 0){
    response->data = NULL;
    return 0;
  }

  response->data = (uint8_t*)malloc(response->size);
  if(read(fd, response->data, response->size) < 0){
    free(response->data);
    return -1;
  }

  return 0;
}

// SETUP FUNCTIONS ================================================================
void setup_conn_fd(char *host, in_port_t port) {
  struct hostent *host_entry;
  struct sockaddr_in sock_addr;

  // create socket
  if ((conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("socket");
    exit(1);
  }

  // get host address
  if ((host_entry = gethostbyname(host)) == NULL) {
    perror("gethostbyname");
    exit(1);
  }

  // setup socket address
  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  memcpy(&sock_addr.sin_addr, host_entry->h_addr_list[0], host_entry->h_length);
  sock_addr.sin_port = htons(port);

  // connect to host
  if (connect(conn_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
    perror("connect");
    exit(1);
  }
}

// signal handler for SIGINT
void handle_sigint(int sig) {
  if (sig == SIGINT) {
    exit(0);
  }
}

// exit handler for connection file descriptor
void handle_exit() {
  if (conn_fd != -1) {
    close(conn_fd);
  }
}

// signal handler setup for SIGINT
void setup_signal_handler() {
  struct sigaction act;
  act.sa_handler = handle_sigint;
  sigaction(SIGINT, &act, NULL);
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

void print_response(Response *response){
  printf("Code: %u, Size: %u, Data: ", response->code, response->size);
  for(size_t i = 0; i < response->size; i++){
    printf("%02x ", response->data[i]);
  }
  printf("\n");
}

void print_request(Request *request){
  printf("User: %u, Size: %u, Action: %u, Data: ", request->user, request->size, request->action);
  for(size_t i = 0; i < request->size; i++){
    printf("%02x ", request->data[i]);
  }
  printf("\n");
}