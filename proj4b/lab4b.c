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

// options
char scale = 'f';
int period = 1;
char log[];

struct pollfd poll;

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
    {"scale", required_argument, NULL, 's'},
    {"period", required_argument, NULL, 'p'},
    {"log", required_argument, NULL, 'l'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "s:p:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 's': scale = atoi(optarg); break;
        case 'p': period = atoi(optarg); break;
        case 'l': log = atoi(optarg); break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--scale=[f/c]\
                                  \n\t--period=seconds\n");
  			          exit(1);
  			          break;

    }
  }
}

void setupPoller() {
  poll.fd = 0;
  poll.events = POLLIN;
}

void run() {
  int status;

  while(1) {
    if (poll.revents & POLLIN) {
      modify();
    }


    wait(1000);
  }
}

void modify() {
  printf("hello!");
}

int main(int argc, char *argv[]) {
	processArguments(argc, argv);
  setupPoller();
  run();
}