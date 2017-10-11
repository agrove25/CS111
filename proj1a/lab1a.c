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

struct termios original;
bool activeShell = false;
char buffer[256];

int toShell[2];     // (rear: child, front: parent -- parent->child)
int fromShell[2];   // (rear: parent, front: child -- child->parent)
pid_t process_id;
struct pollfd polls[2];

void handleError(char loc[200], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

// REMEMBER: IF YOU CHANGE SOMETHING HERE, CHANGE THE SHELL VERSION TOO
bool read_and_write(int in_fd, int out_fd) {
  ssize_t bytes = read(in_fd, buffer, 256);
  if (bytes < 0) {
    handleError("read_and_write (read)", errno);
  }

  for (int i = 0; i < bytes; i++) {
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
// REMEMBER.

// The read_and_write to the shell (due to small differences like \r)
// and double outputs. This is only used for the inputting of text
// the shell.
bool shell_read_and_write (int in_fd, int out_fd) {
  ssize_t bytes = read(in_fd, buffer, 256);
  if (bytes < 0) {
    handleError("shell_read_and_write (read)", errno);
  }
  int i = 0;
  while (i < bytes) {
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
      if (write(1, output, 2) < 0) {
        handleError("shell_read_and_write (write, \\r\\n)", errno);
      }
      if (write(out_fd, output + 1, 1) < 0) {
        handleError("shell_read_and_write (out_fd write, \\n)", errno);
      }

      i++;
      continue;
    }

    if (write (1, buffer + i, 1) < 0) {
      handleError("shell_read_and_write (write)", errno);
    }
    if (write (out_fd, buffer + i, 1) < 0) {
      handleError("shell_read_and_write (out_fd write)", errno);
    }

    i++;
  }

  for (int i = 0; i < bytes; i++) {
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
      if (write(1, output, 2) < 0) {
        handleError("shell_read_and_write (write, \\r\\n)", errno);
      }
      if (write(out_fd, output + 1, 1) < 0) {
        handleError("shell_read_and_write (out_fd write, \\n)", errno);
      }

      continue;
    }

    if (write (1, buffer + i, 1) < 0) {
      handleError("shell_read_and_write (write)", errno);
    }
    if (write (out_fd, buffer + i, 1) < 0) {
      handleError("shell_read_and_write (out_fd write)", errno);
    }
  }

  return true;
}


// helper function that is called at the end by atexit in adjust
void restore_terminal() {
  if (tcsetattr(STDIN_FILENO, TCSANOW, &original) < 0) {
    handleError("restore_terminal (tcsetattr)", errno);
  }
}

void process_shutdown() {
  int status;

  while (polls[1].revents & POLLIN) {
    if ((polls[0].revents & (POLLHUP | POLLERR)) ||
        (polls[0].revents & (POLLHUP | POLLERR))) {
      break;
    }
    if (!read_and_write(fromShell[0], 1)) {
      close(fromShell[0]);
      break;
    }
  }

  close(toShell[0]);
  close(toShell[1]);
  close(fromShell[0]);
  close(fromShell[1]);

  waitpid(process_id, &status, 0);
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d",
         (status & 0x007f), (status & 0xff00));
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

/* This is version 1. It doesn't work for parts C and D */
/********************************************************
void read_and_write(int in_fd, int out_fd) {
  fprintf(stderr, "Reading from: %d; Writing to: %d", in_fd, out_fd);

  ssize_t bytes = read(in_fd, buffer, 1);
  if (bytes < 0) {
    handleError("read_and_write (read)", errno);
  }

  while (bytes > 0) {
    for (int i = 0; i < bytes; i++) {
      if (buffer[i] == 4) {
        exit(0);
      }
      else if (buffer[i] == '\r' || buffer[i] == '\n') {
        char output[2] = {'\r', '\n'};
        if (write(out_fd, output, 2) < 0) {
          handleError("read_and_write (write, \\r\\n)", errno);
        }
        if (activeShell && write(toShell[1], output, 2) < 0) {
          handleError("read_and_write (toShell write, \\r\\n)", errno);
        }

        continue;
      }

      if (write (out_fd, buffer + i, 1) < 0) {
        handleError("read_and_write (write)", errno);
      }
      if (activeShell && write(toShell[1], buffer + i, 2) < 0) {
        handleError("read_and_write (toShell write)", errno);
      }
    }

    bytes = read(in_fd, buffer, 256);
    if (bytes < 0) {
      handleError("read_and_write (read, within loop)", errno);
    }
  }
}
*****************************************************************/

void normalStart() {
  // this is an infinite loop, because basically I want to trigger
  // read_and_write until it returns false.

  while (1) {
    if (!read_and_write(0, 1)) {
      exit(0);
    }
  }
}

// contains the polling loop to switch read_and_writes
void shellStart() {
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
        if (!shell_read_and_write(0, toShell[1])) {
          close(toShell[1]);
          exit(0);
        }
      }
      if (polls[1].revents & POLLIN) {
        if (!read_and_write(fromShell[0], 1)) {
          close(fromShell[0]);
          exit(0);
        }
      }
      if ((polls[0].revents & (POLLHUP | POLLERR)) ||
          (polls[0].revents & (POLLHUP | POLLERR))) {
        exit(0);
      }
    }
  }

  read_and_write(fromShell[0], 1);
}

void activateShell() {
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

    polls[0].fd = 0;
    polls[0].events = POLLIN;

    polls[1].fd = fromShell[0];
    polls[1].events = POLLIN;

    atexit(process_shutdown);

    shellStart();
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  char opt;

  static struct option long_options[] =
	{
    {"shell", no_argument, NULL, 's'},
		{0, 0, 0, 0}
	};

  while ((opt = getopt_long(argc, argv, "s", long_options, NULL)) != -1) {
    switch (opt) {
      case 's': activeShell = true; break;
      default  : fprintf(stderr, "Usage : lab0 [OPTION] = [ARGUMENT]\n");
			           fprintf(stderr, "OPTION: \n\t--shell\n");
			           exit(1);
			           break;
    }
  }

  adjust_terminal();

  if (activeShell) {
    activateShell();
  }

  normalStart();  // the program doesn't get to this point if we use --shell
}
