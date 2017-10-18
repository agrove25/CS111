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
MCRYPT td;

// Shell Stuff
struct termios original;
bool activeShell = false;
char buffer[256];

int toShell[2];     // (rear: child, front: parent -- parent->child)
int fromShell[2];   // (rear: parent, front: child -- child->parent)
pid_t process_id;
struct pollfd polls[2];

// Server-Client Stuff
int port = -1;
int serv_fd, cli_fd;
struct sockaddr_in serv, cli;

// Function declerations to keep some semblance of organization
bool read_and_write(int in_fd, int out_fd, bool isSending);
bool shell_read_and_write (int in_fd, int out_fd, bool isSending);
void restore_terminal();
void process_shutdown();
void prepareEncryption();

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

void signal_handler() {
  fprintf(stderr, "encountered the SIGPIPE error\n");
  exit(0);
}

void processArguments(int argc, char* argv[]) {
  char opt;

  static struct option long_options[] =
  {
    {"port", required_argument, NULL, 'p'},
    {"encrypt", no_argument, NULL, 'e'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "p:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 'p': port = atoi(optarg); break;

        case 'e': encrypt_flag = true;
                  prepareEncryption();
                  break;

        default  : fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			           fprintf(stderr, "OPTION: \n\t--port=portnum\n");
  			           exit(1);
  			           break;

    }
  }

  if (port == -1) {
    fprintf(stderr, "Port option is mandatory.\n");
    handleError("processArguments (port)", errno);
  }
  else if (port <= 1024 || port >= 65535) {
    fprintf(stderr, "Port number must be between 1024 and 65535.\n");
    exit(1);
  }
}

void prepareEncryption() {
  char* key;
  char* IV;
  int keysize = 16;
  key = calloc(1, keysize);

  int keyfd = open("my.key", O_RDONLY);
  if (keyfd < 0) {
    handleError("prepareEncryption (open)", errno);
  }

  read(keyfd, key, keysize);
  td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
  if (td == MCRYPT_FAILED) {
    handleError("prepareEncryption (mcrypt_module_open)", errno);
  }

  IV = (char*) malloc(mcrypt_enc_get_iv_size(td));
  if (IV == NULL) {
    handleError("prepareEncryption (malloc)", errno);
  }
  for (int i = 0; i < mcrypt_enc_get_iv_size(td); i++) {
    IV[i] = rand();
  }

  int status = mcrypt_generic_init(td, key, keysize, NULL);
  if (status < 0) {
    mcrypt_perror(status);
    handleError("prepareEncryption (generic_init)", errno);
  }

  free(key);
}

void startServer() {
  serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_fd == -1) {
    handleError("startServer (declaring socket)", errno);
  }
  fprintf(stderr, "Created socket\n");

  serv.sin_family = AF_INET;
  serv.sin_addr.s_addr = INADDR_ANY;
  serv.sin_port = htons( port );

  if (bind(serv_fd, (struct sockaddr *) &serv, sizeof(serv)) < 0)
    handleError("startServer (bind socket)", errno);

  listen(serv_fd, 5);
  fprintf(stderr, "Waiting for clients...\n");

  // Accepting Conenctions...
  int addr_len = sizeof(cli);
  cli_fd = accept(serv_fd, (struct sockaddr *) &cli, (socklen_t*) &addr_len);
  if (cli_fd == -1) {
    handleError("startServer (accepting client)", errno);
  }
  fprintf(stderr, "Accepted client\n");
}

// sets the global var original and changes terminal attributes
void adjust_terminal() {
  int result;
  struct termios modified;

  result = tcgetattr (STDIN_FILENO, &original);
  if (result < 0) {
    handleError("adjust_terminal (tcgetattr)", errno);
  }

  atexit(restore_terminal);

  result = tcgetattr (STDIN_FILENO, &modified);
  if (result < 0) {
    handleError("adjust_terminal (tcgetattr)", errno);
  }

  modified.c_iflag = ISTRIP;
  modified.c_oflag = 0;
  modified.c_lflag = 0;

  if (tcsetattr(STDIN_FILENO, TCSANOW, &modified) < 0) {
    handleError("adjust_terminal (tcsetattr)", errno);
  }
}

void activateShell() {
  signal(SIGPIPE, signal_handler);

  if (pipe(toShell) != 0) {
    handleError("main (pipe to shell)", errno);
  }
  if (pipe(fromShell) != 0) {
    handleError("main (pipe from shell)", errno);
  }

  process_id = fork();
  if (process_id < 0) {
    handleError("main (fork)", errno);
  }
  else if (process_id == 0) {
    // Child process
    close(toShell[1]);
    close(fromShell[0]);

    dup2(toShell[0], 0);
    dup2(fromShell[1], 1);
    dup2(fromShell[1], 2);

    close(toShell[0]);
    close(fromShell[1]);

    if (execvp("/bin/bash", NULL)) {
      handleError("main (execvp)", errno);
    }
  }
  else {
    // Parent process
    close(toShell[0]);
    close(fromShell[1]);

    // Server additions
    close(0);
    close(1);

    dup(cli_fd);
    dup(cli_fd);

    close(cli_fd);

    // Polling initializtion
    polls[0].fd = 0;
    polls[0].events = POLLIN;

    polls[1].fd = fromShell[0];
    polls[1].events = POLLIN;

    atexit(process_shutdown);
  }
}

// contains the polling loop to switch read_and_writes
void runProcess() {
  // also an infinite loop, because the polling if statement will control
  // when the program exits
  int poll_result;

  while (1) {
    if ((poll_result = poll(polls, 2, 0)) < 0) {
      handleError("shellStart (poll)", errno);
    }
    else if (poll_result > 0) {
      // this branch controls whether we read from shell and outputs into
      // stdout or whether we read from stdin and output into the shell

      if (polls[0].revents & POLLIN) {
        if (!shell_read_and_write(0, toShell[1], false)) {
          close(toShell[1]);
          exit(0);
        }
      }
      if (polls[1].revents & POLLIN) {
        if (!read_and_write(fromShell[0], 1, true)) {
          close(fromShell[0]);
        }
      }
      if ((polls[0].revents & (POLLHUP | POLLERR)) ||
          (polls[1].revents & (POLLHUP | POLLERR))) {
        exit(0);
      }
    }
  }

  read_and_write(fromShell[0], 1, true);
}

// REMEMBER: IF YOU CHANGE SOMETHING HERE, CHANGE THE SHELL VERSION TOO
bool read_and_write(int in_fd, int out_fd, bool isSending) {
  ssize_t bytes = read(in_fd, buffer, 256);
  if (bytes < 0) {
    handleError("read_and_write (read)", errno);
  }

  int i;
  for (i = 0; i < bytes; i++) {
    if (encrypt_flag) {
      fprintf(stderr, "ENTERED: ");
      fprintf(stderr, buffer);
      fprintf(stderr, "\n");

      if (isSending) {
        if (mcrypt_generic(td, &buffer[i], 1) < 0) {
          handleError("read_and_write (encrypt)", errno);
        }

        fprintf(stderr, "ENCRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
      else {
        if (mdecrypt_generic(td, &buffer[i], 1) < 0) {
          handleError("read_and_write (decrypt)", errno);
        }

        fprintf(stderr, "DECRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
    }

    if (buffer[i] == 4) {
      return false;
    }
    else if (buffer[i] == '\r' || buffer[i] == '\n') {
      char output[2] = {'\r', '\n'};
      if (write(out_fd, output, 2) < 0) {
        handleError("read_and_write (write, \\r\\n)", errno);
      }

      continue;
    }

    if (write (out_fd, buffer + i, 1) < 0) {
      handleError("read_and_write (write)", errno);
    }
  }

  return true;
}

// The read_and_write to the shell (due to small differences like \r)
// and double outputs. This is only used for the inputting of text
// the shell.
// REMEMBER.
bool shell_read_and_write (int in_fd, int out_fd, bool isSending) {
  ssize_t bytes = read(in_fd, buffer, 256);
  if (bytes < 0) {
    handleError("shell_read_and_write (read)", errno);
  }

  char copyBuffer[256];
  strcpy(copyBuffer, buffer);     // a crappy hack to get encryption to work
                                  // because we need to print the unecrypted
                                  // to the shell.

  int i;
  for (i = 0; i < bytes; i++) {
    if (encrypt_flag) {
      fprintf(stderr, "ENTERED: ");
      fprintf(stderr, buffer);
      fprintf(stderr, "\n");

      if (isSending) {
        if (mcrypt_generic(td, &buffer[i], 1) < 0) {
          handleError("read_and_write (encrypt)", errno);
        }

        fprintf(stderr, "ENCRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
      else {
        if (mdecrypt_generic(td, &buffer[i], 1) < 0) {
          handleError("read_and_write (decrypt)", errno);
        }

        fprintf(stderr, "DECRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
    }

    if (buffer[i] == 3) {
      if (write(1, "^c", 2) < 0) {
        handleError("shell_read_and_write (write, ^c)", errno);
      }

      kill(process_id, SIGINT);
      i++;
      continue;
    }
    if (buffer[i] == 4) {
      return false;
    }
    else if (buffer[i] == '\r' || buffer[i] == '\n') {
      char output[2] = {'\r', '\n'};

      if (write(out_fd, output + 1, 1) < 0) {
        handleError("shell_read_and_write (out_fd write, \\n)", errno);
      }

      if (!encrypt_flag) {
        if (isSending) {
          if (mcrypt_generic(td, &output[0], 1) < 0) {
            handleError("read_and_write (encrypt)", errno);
          }
        }
        else {
          if (mdecrypt_generic(td, &output[0], 1) < 0) {
            handleError("read_and_write (decrypt)", errno);
          }
        }
      }

      if (write(1, output, 2) < 0) {
        handleError("shell_read_and_write (write, \\r\\n)", errno);
      }

      continue;
    }

    fprintf(stderr, copyBuffer);
    if (write (1, copyBuffer + i, 1) < 0) {
      handleError("shell_read_and_write (write)", errno);
    }
    if (write (out_fd, buffer + i, 1) < 0) {
      handleError("shell_read_and_write (out_fd write)", errno);
    }
  }

  return true;
}

void process_shutdown() {
  int status;

  while (polls[1].revents & POLLIN) {
    if ((polls[0].revents & (POLLHUP | POLLERR)) ||
        (polls[1].revents & (POLLHUP | POLLERR))) {
      break;
    }
    if (!read_and_write(fromShell[0], 1, true)) {
      close(fromShell[0]);
      break;
    }
  }

  close(toShell[0]);
  close(toShell[1]);
  close(fromShell[0]);
  close(fromShell[1]);

  while (waitpid(process_id, &status, WNOHANG) == 0) {

  }
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n",
         (status & 0x007f), (status & 0xff00) >> 8);

  close(cli_fd);
}

// helper function that is called at the end by atexit in adjust
void restore_terminal() {
  if (tcsetattr(STDIN_FILENO, TCSANOW, &original) < 0) {
    handleError("restore_terminal (tcsetattr)", errno);
  }
}

int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  startServer();
  adjust_terminal();
  activateShell();
  runProcess();
  exit(0);
}
