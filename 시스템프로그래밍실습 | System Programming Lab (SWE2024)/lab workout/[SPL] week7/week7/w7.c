#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

long num_signal_to_send;       // passed as an argument
long num_signal_received = 0;  // in receiver/child
long num_ack_received = 0;     // in sender/parent
pid_t pid;

/**
 * @brief Sets up the signal handler
 *
 * @param action sigaction
 * @param signal the signal (e.g. SIGINT, SIGUSR1s, etc.)
 * @param handler the signal handler function associated to the signal
 */
void set_signal(struct sigaction action, int signal, void (*handler)(int)) {
  // TODO: code starts
  action.sa_handler = handler;
  sigaction(signal, &action, NULL);
  // TODO: code ends
}

/**
 * @brief handler in sender process (parent)
 *
 * This handler is called when the sender receives an
 * acknowledgment from the receiver
 *
 * @param sig Assumed to be SIGUSR1
 */
void recv_ack_handler(int sig) {
  // TODO: code start
  num_ack_received++;
  // printf("sender: total remaining signal(s): %ld\n", num_signal_to_send - num_ack_received);
  // TODO: code end
}

/**
 * No need to modify
 *
 * @brief handler in receiver process (child)
 *
 * If receiver receive SIGINT, Receiver prints how many signal
 * it received and then terminates
 *
 * @param sig Assumed to be SIGINT
 */
void terminate_handler(int sig) {
  // No need to modify
  printf("receiver: received %ld signals\n", num_signal_received);
  _exit(0);
}

/**
 * @brief handler in sender
 *
 * checks the number of sent signals and received acknowledgments
 *
 * if these two values are the same, send SIGINT to receiver
 * and then terminate the sender in this function
 *
 * if not, send a custom signal to receiver
 * and then print the number of remaining signals to send
 *
 * (already implementewd)
 * we send back the remaining signals again using `alarm`
 * if the sender does not receive all acknowledgments.
 * @param sig Assumed to be SIGALRM
 */
void sending_handler(int sig) {
  // TODO: code starts
  if(num_signal_to_send == num_ack_received){
    printf("all signals have been sent!\n");
    kill(pid, SIGINT); 
    exit(0);
  }else{
    printf("sender: total remaining signal(s): %ld\n", num_signal_to_send - num_ack_received);
    kill(pid, SIGUSR1);
  }
  alarm(1);
  // TODO: code end
}

/**
 * @brief handler in receiver
 *
 * occurs when receiver receives a signal from the sender through SIGUSR1
 *
 * this function sends back an acknowledgment to the sender through SIGUSR1
 *
 * @param sig Assumed to be SIGUSR1
 */
void sending_ack(int sig) {
  // TODO: code start
  num_signal_received++;
  printf("receiver: received signal #%ld and sending ack\n", num_signal_received);
  kill(pid, SIGUSR1);
  // TODO:code end
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <number of signals to send>\n", argv[0]);
    exit(1);
  }

  num_signal_to_send = strtol(argv[1], NULL, 10);

  if (num_signal_to_send <= 0) {
    fprintf(stderr, "Invalid number of signals to send\n");
    exit(1);
  }

  printf("number of signals to send: %ld\n", num_signal_to_send);

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  // sigemptyset(&action.sa_mask);
  // action.sa_flags = 0;

  switch (pid = fork()) {
    case -1:
      perror("fork failed");
      exit(1);
    case 0:             // Child process (receiver)
      pid = getppid();  // Note: PID is now parent process (sender)!

      // TODO: code start

      // set up sending_ack signal handler (i.e. SIGUSR1)
      set_signal(action, SIGUSR1, sending_ack);

      // set up terminate_handler signal handler (i.e. SIGINT)
      set_signal(action, SIGINT, terminate_handler);

      
      
      // TODO: code end
      while (1)
        ;
      break;
    default:
      // Parent process (sender)
      // pid = child id

      // TODO: code start

      // set up recv_ack_handler signal handler (i.e. SIGUSR1)
      set_signal(action, SIGUSR1, recv_ack_handler);

      // signal  sending_handler signal handler (i.e. SIGALRM)
      set_signal(action, SIGALRM, sending_handler);

      // TODO: code end

      // send back remaining signals after 1s
      // if sender doesn't receive all acks using alarm
      alarm(1);

      while (1)
        ;
      break;
  }

  return 0;
}