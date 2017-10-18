// NAME: Andrew Grove
// EMAIL: koeswantoandrew@gmail.com
// ID: 304785991

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <string.h>

#include <getopt.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <poll.h>

// logging
#include <sys/stat.h>
#include <fcntl.h>

// cryption
#include <mcrypt.h>

// network
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr

int port = -1;
bool log_flag = false, encrypt_flag = false;
int log_fd = -1;

struct termios original;
char buffer[256];
struct pollfd polls[2];

MCRYPT encrypt_fd, decrypt_fd;

// Client-Server Variables
int serv_fd;
struct sockaddr_in server;

// Function Definitions
bool read_and_write(int in_fd, int out_fd, bool isSending);
void restore_terminal();
void prepareEncryption();

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

void processArguments(int argc, char* argv[]) {
  char opt;

  static struct option long_options[] =
  {
    {"port", required_argument, NULL, 'p'},
    {"log", required_argument, NULL, 'l'},
    {"encrypt", no_argument, NULL, 'e'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "p:l:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 'p': port = atoi(optarg); break;

        case 'l': log_flag = true;
                  log_fd = creat(optarg, 0666);
                  // char buffer[4] = "test";
                  // write(log_fd, buffer, strlen(buffer));
                  break;

        case 'e': encrypt_flag = true;
                  prepareEncryption();
                  break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
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
  struct stat key_stat;
  int key_fd = open("my.key", O_RDONLY);
  fstat(key_fd, &key_stat);
  char *key = (char*) malloc(key_stat.st_size * sizeof(char));
  read(key_fd, key, key_stat.st_size);



  encrypt_fd = mcrypt_module_open("blowfish", NULL, "ofb", NULL);
  if (encrypt_fd < 0) {
    handleError("prepareEncryption (encrypt open)", errno);
  }
  if (mcrypt_generic_init(encrypt_fd, key, key_stat.st_size, NULL) < 0) {
    handleError("prepareEncryption (encrypt init)", errno);
  }

  decrypt_fd = mcrypt_module_open("blowfish", NULL, "ofb", NULL);
  if (decrypt_fd < 0) {
    handleError("prepareEncryption (decrypt open)", errno);
  }
  if (mcrypt_generic_init(decrypt_fd, key, key_stat.st_size, NULL) < 0) {
    handleError("prepareEncryption (decrypt init)", errno);
  }

  free(key);
}

void connectServer() {
  serv_fd = socket(AF_INET , SOCK_STREAM , 0);
  if (serv_fd == -1)
  {
      handleError("connectServer (creating socket)", errno);
  }
  fprintf(stderr, "Connected to server.\n");

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( port );

  if (connect(serv_fd , (struct sockaddr *)&server , sizeof(server)) < 0)
  {
      handleError("connectServer (connect)", errno);
  }
  fprintf(stderr, "Connected.\n");
}

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

void setPoller() {
  polls[0].fd = 0;
  polls[0].events = POLLIN;

  polls[1].fd = serv_fd;
  polls[1].events = POLLIN;
}

void normalStart() {
  int poll_result;

  while (1) {
    if ((poll_result = poll(polls, 2, 0)) < 0) {
      handleError("normalStart (poll)", errno);
    }
    else if (poll_result > 0) {
      // this branch controls whether we read from shell and outputs into
      // stdout or whether we read from stdin and output into the shell

      if (polls[0].revents & POLLIN) {
        if (!read_and_write(0, serv_fd, true)) {
          break;
        }
      }
      if (polls[1].revents & POLLIN) {
        if (!read_and_write(serv_fd, 1, false)) {
          exit(0);
        }
      }
      if ((polls[0].revents & (POLLHUP | POLLERR)) ||
          (polls[1].revents & (POLLHUP | POLLERR))) {
        exit(0);
      }
    }
  }

  // flush all remaining output
  while(1) {
    if (polls[1].revents & POLLIN) {
      if (!read_and_write(serv_fd, 1, false)) {
        exit(0);
      }
    }
    else {
      exit(0);
    }

    if ((polls[0].revents & (POLLHUP | POLLERR)) ||
        (polls[1].revents & (POLLHUP | POLLERR))) {
      exit(0);
    }
  }
}

void restore_terminal() {
  close(serv_fd);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &original) < 0) {
    handleError("restore_terminal (tcsetattr)", errno);
  }
}

char encrypt(char *c) {
  if(mcrypt_generic(encrypt_fd, c, 1) != 0)
    handleError("encrypt", errno);
}
char decrypt(char *c) {
  if(mdecrypt_generic(encrypt_fd, c, 1) != 0)
    handleError("decrypt", errno);
}

bool read_and_write(int in_fd, int out_fd, bool isSending) {
  ssize_t bytes = read(in_fd, buffer, 256);
  if (bytes < 0) {
    handleError("read_and_write (read)", errno);
  }

  if (log_flag) {
    int n;
    char log_prefix[50];

    if (isSending) {
      n = sprintf(log_prefix, "SENT %d bytes: ", strlen(buffer));
    }
    else {
      n = sprintf(log_prefix, "RECEIVED %d bytes: ", strlen(buffer));
    }

    write(log_fd, log_prefix, n);
    write(log_fd, buffer, strlen(buffer));
    write(log_fd, "\n", 1);
  }

  int i;
  for (i = 0; i < bytes; i++) {
    if (encrypt_flag) {
      fprintf(stderr, "ENTERED: ");
      fprintf(stderr, buffer);
      fprintf(stderr, "\n");

      if (isSending) {
        encrypt(buffer+i);

        fprintf(stderr, "ENCRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
      else {
        decrypt(buffer+i);

        fprintf(stderr, "DECRYPT: ");
        fprintf(stderr, buffer);
        fprintf(stderr, "\n");
      }
    }

    decrypt(buffer+i);
    fprintf(stderr, "DECRYPT: ");
    fprintf(stderr, buffer);
    fprintf(stderr, "\n");

    if (write (out_fd, buffer + i, 1) < 0) {
      handleError("read_and_write (write)", errno);
    }
    if (buffer[i] == 4 || buffer[i] == 3) {
      return false;
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  connectServer();
  adjust_terminal();
  setPoller();
  normalStart();
}
