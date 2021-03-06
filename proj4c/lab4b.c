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
#include <time.h>
#include <fcntl.h>

// options
bool running = true;
char scale = 'f';
int period = 1;
char* logFile;

struct pollfd poller;

mraa_aio_context tempSensor;
mraa_gpio_context button;

void shutdown();
void setupLogging();
bool parse();

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

  while ( (opt = getopt_long(argc, argv, "s:p:l:", long_options, NULL)) != 255 ) {
    switch(opt) {
        case 's': scale = optarg[0]; break;
        case 'p': period = atoi(optarg); break;
        case 'l': logFile = optarg; setupLogging(); break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--scale=[f/c]\
                                  \n\t--period=seconds\
                                  \n\t--log=logfile\n");
  			          exit(1);
  			          break;

    }
  }
}

void setupLogging() {
  int ofd = creat(logFile, 0666);
  if (ofd < 0) {
    int err = errno;
    handleError("setupLogging", err);
  }

  close(1);
  dup(ofd);
  close(ofd);

}

void setupPoller() {
  poller.fd = 0;
  poller.events = POLLIN;
}

void setupSensors() {
  tempSensor = mraa_aio_init(1);
  if (tempSensor == NULL) {
    handleError("setupSensors (tempSensor)", errno);
  }

  button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);
  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &shutdown, NULL);
}

void run() {
  int poll_result;

  while(1) {
    if (running)
        getTemperature();

    poll_result = poll(&poller, 1, 0);
    if (poll_result < 0) {
      handleError("run (poll)", errno);
    }

    if (poll_result > 0 && (poller.revents & POLLIN)) {
      modify();
    }

    
    if (running) {
      sleep(period);
    }
  }
}

void modify() {
  char string[256];
  int strSize = read(0, string, 256);
  string[strSize] = '\0';
  
  int start = 0;
  int j;
  for (j = 0; j < strSize; j++) {
    if (string[j] == '\n') {
      string[j] = '\0';

      if (parse(string + start)) {
        write(1, string + start, j - start);
        write(1, "\n", 1);
      }

      start = j + 1;

    }
  }

}

bool parse(char string[256]) {
  if (strcmp(string, "SCALE=F") == 0) {
    scale = 'f';
    //fprintf(stderr, "Changing Scale to Farenheit...\n");
  }
  else if (strcmp(string, "SCALE=C") == 0) {
    scale = 'c';
    //fprintf(stderr, "Changing Scale to Celsius...\n");
  }
  else if (strcmp(string, "SCALE=F") == 0) {
    scale = 'f';
    //fprintf(stderr, "Changing Scale to Farenheit...\n");
  }
  else if (strcmp(string, "START") == 0) {
    running = true;
    //fprintf(stderr, "Starting...\n");
  }
  else if (strcmp(string, "STOP") == 0) {
    running = false;
    //fprintf(stderr, "Stopping...\n");
  }
  else if (strcmp(string, "OFF") == 0) {
    char buffer[5] = "OFF\n";
    write(1, buffer, 5);
    shutdown();
    exit(0);
  }
  else {
    int i;
    char checker[8] = "PERIOD=";
    for (i = 0; i < 7; i++) {
      if (string[i] != checker[i]) {
        return false;
      }
    }

    char number[10];
    for (; true; i++) {
      if (isdigit(string[i]))
        number[i-7] = string[i];
      else if (string[i] == '\0')
        break;
      else
        return false;
    }

    number[i-7] = '\0';
    period = atoi(number);
    //fprintf(stderr, "Changing Period to %d\n", period);
  }

  return true;
}

void getTemperature() {
  float a = mraa_aio_read(tempSensor);
  float R = 1023.0/a-1.0;
  R = 100000*R;

  float temperature = 1.0/(logf(R/100000)/4275+1/298.15)-273.15; // convert to temperature via datasheet

  time_t rawTime = time(NULL);
  struct tm * currTime;
  currTime = localtime(&rawTime);

  if (scale == 'f') {
     temperature = temperature * 1.8 + 32;
  }



  char buffer[50];
  int bufferSize = sprintf(buffer, "%.2d:%.2d:%.2d %.1f\n", currTime->tm_hour, currTime->tm_min, currTime->tm_sec, temperature);
  write(1, buffer, bufferSize);
  //fprintf(stderr, "Temperature gotten..\n");
  return;
}

void shutdown() {
  //fprintf(stderr, "Button Pressed\n");
  time_t rawTime = time(NULL);
  struct tm * currTime;
  currTime = localtime(&rawTime);

  char buffer[50];
  int bufferSize = sprintf(buffer, "%.2d:%.2d:%.2d SHUTDOWN\n", currTime->tm_hour, currTime->tm_min, currTime->tm_sec);
  write(1, buffer, bufferSize);

  exit(0);
}


int main(int argc, char *argv[]) {
	processArguments(argc, argv);
  setupPoller();
  setupSensors();
  run();
}
