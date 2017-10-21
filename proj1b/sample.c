// NAME: Andrew Grove
// EMAIL: koeswantoandrew@gmail.com
// ID: 304785991

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#include <getopt.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <poll.h>

// cryption
#include <mcrypt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

// network
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

// cryption gloabl variables
bool encrypt_flag = false;
MCRYPT etd, dtd;

// Shell Stuff
struct termios original;
char buffer[256];

int toShell[2];     // (rear: child, front: parent -- parent->child)
int fromShell[2];   // (rear: parent, front: child -- child->parent)
pid_t process_id;
struct pollfd polls[2];

// Server-Client Stuff
int port = -1;
int serv_fd, cli_fd;
struct sockaddr_in serv, cli;
void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

void restore_terminal() {
  if (tcsetattr(STDIN_FILENO, TCSANOW, &original) < 0) {
    handleError("restore_terminal (tcsetattr)", errno);
  }
}

void adjust_terminal() {
  int result;
  struct termios modified;

  result = tcgetattr (STDIN_FILENO, &original);
  if (result < 0) {
    handleError("adjust_terminal (tcgetattr, original)", errno);
  }

  atexit(restore_terminal);

  result = tcgetattr (STDIN_FILENO, &modified);
  if (result < 0) {
    handleError("adjust_terminal (tcgetattr, modified)", errno);
  }

  modified.c_iflag = ISTRIP;
  modified.c_oflag = 0;
  modified.c_lflag = 0;

  if (tcsetattr(STDIN_FILENO, TCSANOW, &modified) < 0) {
    handleError("adjust_terminal (tcsetattr)", errno);
  }
}

int main() {
  adjust_terminal();
  fprintf(stderr, "SUCCESS!");
  return 0;
}
