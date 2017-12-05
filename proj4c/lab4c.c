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

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

// options
bool running = true;
char scale = 'f';
int period = 1;
char* logFile;

struct pollfd poller;

mraa_aio_context tempSensor;

// server stuff
int sockfd;
int port = 18000;
char* host = "lever.cs.ucla.edu";

struct hostent *server;
struct sockaddr_in server_in;


void setupLogging();

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
    fprintf(stderr, "%d", opt);

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

void createSocket() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    int err = errno;
    handleError("createSocket (create sockfd)", err);
  }

  server = gethostbyname(host);
  if (server == NULL) {
    int err = errno;
    handleError("createSocket (create server)", err);
  }

  memset((char*) &server_in, 0, sizeof(server_in));
  server_in.sin_family = AF_INET;
  memcpy((char*) &server_in.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
  server_in.sin_port = htons(port);

  if (connect(sockfd, (struct sockaddr *)&server_in , sizeof(server_in)) < 0)
  {
      int err = errno;
      handleError("createSocket (connect)", err);
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

    if(running) {
      getTemperature();
      sleep(period);
    }
  }
}

void modify() {
  char string[256];
  int strSize = read(sockfd, string, 256);
  string[strSize] = '\0';

  if (strcmp(string, "SCALE=F\n") == 0) {
    scale = 'f';
    //fprintf(stderr, "Changing Scale to Farenheit...\n");
  }
  else if (strcmp(string, "SCALE=C\n") == 0) {
    scale = 'c';
    //fprintf(stderr, "Changing Scale to Celsius...\n");
  }
  else if (strcmp(string, "SCALE=F\n") == 0) {
    scale = 'f';
    //fprintf(stderr, "Changing Scale to Farenheit...\n");
  }
  else if (strcmp(string, "START\n") == 0) {
    running = true;
    //fprintf(stderr, "Starting...\n");
  }
  else if (strcmp(string, "STOP\n") == 0) {
    running = false;
    //fprintf(stderr, "Stopping...\n");
  }
  else if (strcmp(string, "OFF\n") == 0) {
    getTemperature();
    write(1, string, strSize);
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
      if (isdigit(string[i]))
        number[i-7] = string[i];
      else if (string[i] == '\n')
        break;
      else
        return;
    }

    number[i-7] = '\0';
    period = atoi(number);
    //fprintf(stderr, "Changing Period to %d\n", period);
  }

  write(1, string, strSize);
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

  fprintf(1, "%.2d:%.2d:%.2d %.1f\n", currTime->tm_hour, currTime->tm_min, currTime->tm_sec, temperature);
  fprintf(sockfd, "%.2d:%.2d:%.2d %.1f\n", currTime->tm_hour, currTime->tm_min, currTime->tm_sec, temperature);
  //fprintf(stderr, "Temperature gotten..\n");
  return;
}

int main(int argc, char *argv[]) {
	processArguments(argc, argv);
  createSocket();
  setupPoller();
  setupSensors();
  run();
}
