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
#include <poll.h>
#include <string.h>
#include <math.h>

#include <mraa.h>
#include <signal.h>

// options
bool running = true;
char scale = 'f';
int period = 1;
char* logFile;

struct pollfd poller;

mraa_aio_context tempSensor;
mraa_gpio_context button;

void buttonPress();

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
        case 's': scale = optarg[0]; break;
        case 'p': period = atoi(optarg); break;
        case 'l': logFile = optarg; break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--scale=[f/c]\
                                  \n\t--period=seconds\n");
  			          exit(1);
  			          break;

    }
  }
}

void setupPoller() {
  poller.fd = 0;
  poller.events = POLLIN;
}

void setupSensors() {
  tempSensor = mraa_aio_init(0);

  button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);
  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &buttonPress, NULL);
}

void run() {
  int poll_result;

  while(1) {
    poll_result = poll(&poller, 1, 0);
    if (poll_result < 0) {
      handleError("run (poll)", errno);
    }

    if (poll_result > 0 && (poller.revents & POLLIN)) {
      modify();
    }

    getTemperature();
    sleep(1);

  }
}

void modify() {
  char string[256];
  int strSize = read(0, string, 256);
  string[strSize] = '\0';

  write(0, string, strSize);
  fprintf(stderr, "fprintf: %s\n", string);

  if (strcmp(string, "SCALE=F\n") == 0) {
    scale = 'f';
    fprintf(stderr, "Changing Scale to Farenheit...\n");
  }
  else if (strcmp(string, "SCALE=C\n") == 0) {
    scale = 'c';
    fprintf(stderr, "Changing Scale to Celsius...\n");
  }
  else if (strcmp(string, "SCALE=C\n") == 0) {
    scale = 'c';
    fprintf(stderr, "Changing Scale to Celsius...\n");
  }
  else if (strcmp(string, "START\n") == 0) {
    running = true;
    fprintf(stderr, "Starting...\n");
  }
  else if (strcmp(string, "STOP\n") == 0) {
    running = false;
    fprintf(stderr, "Stopping...\n");
  }
  else if (strcmp(string, "OFF\n") == 0) {
    getTemperature();
    exit(0);
  }
  else {
    int i;
    char checker[8] = "PERIOD=";
    for (i = 0; i < 7; i++) {
      if (string[i] != checker[i]) {
        return;
      }
    }

    char number[10];
    for (; true; i++) {
      fprintf(stderr, "%c", string[i]);
      if (isdigit(string[i]))
        number[i-7] = string[i];
      else if (string[i] == '\n')
        break;
      else
        return;
    }

    number[i-7] = '\0';
    period = atoi(number);
    fprintf(stderr, "Changing Period to %d\n", period);
  }
}

void getTemperature() {
  return;
}

void buttonPress() {
  fprintf(stderr, "Button Pressed");
  getTemperature();
  exit(0);
}

int main(int argc, char *argv[]) {
	processArguments(argc, argv);
  setupPoller();
  setupSensors();
  run();
}
