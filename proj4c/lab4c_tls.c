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

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>

// options
bool running = true;
char scale = 'f';
int period = 1;
char* logFile = "";

struct pollfd poller;

mraa_aio_context tempSensor;

// server stuff
int sockfd;
int port = 18000;
char* host = "";

struct hostent *server;
struct sockaddr_in server_in;
char* id = "";

SSL* ssl;

void setupLogging();
void printID();
bool parse();

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(2);
}

void processArguments(int argc, char* argv[]) {
  char opt;

  bool doubleArg = false;
  int i;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      if (doubleArg) {
        fprintf(stderr, "Invalid arguments.\n");
        exit(1);
      }

      doubleArg = true;
      port = atoi(argv[i]);
      fprintf(stderr, "Port: %d\n", atoi(argv[i]));
    }
  }

  if (!doubleArg) {
    fprintf(stderr, "Requires a port.\n");
    exit(1);
  }


  static struct option long_options[] =
  {
    {"scale", required_argument, NULL, 's'},
    {"period", required_argument, NULL, 'p'},
    {"log", required_argument, NULL, 'l'},
    {"id", required_argument, NULL, 'i'},
    {"host", required_argument, NULL, 'h'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "s:p:l:i:h:", long_options, NULL)) != 255 ) {
    switch(opt) {
        case 's': scale = optarg[0]; break;
        case 'p': period = atoi(optarg); break;
        case 'l': logFile = optarg; setupLogging(); break;
        case 'i': id = optarg; break;
        case 'h': host = optarg; break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
                  fprintf(stderr, "OPTION: \n\t--scale=[f/c]\
                                  \n\t--period=seconds\
                                  \n\t--log=logfile\
                                  \n\t--id=idnum\
                                  \n\t--host=hostid\n");
                  exit(1);
                  break;

    }
  }

  if (id == "") {
    fprintf(stderr, "Requires an id.\n");
    exit(1);
  }
  if (host == "") {
    fprintf(stderr, "Requires a hostname.\n");
    exit(1);
  }
  if (logFile == "") {
    fprintf(stderr, "Requires a logfile.\n");
    exit(1);
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

  // SETTING UP SSL
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
  SSL_CTX* ssl_context = SSL_CTX_new(SSLv23_client_method());
  ssl = SSL_new(ssl_context);

  if (SSL_set_fd(ssl, sockfd) == 0) {
    int err = errno;
    handleError("createSocket (ssl binding)", err);
  }

  if (SSL_connect(ssl) != 1) {
    int err = errno;
    handleError("createSocket (ssl connect)", err);
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
  poller.fd = sockfd;
  poller.events = POLLIN;
}

void setupSensors() {
  tempSensor = mraa_aio_init(1);
  if (tempSensor == NULL) {
    handleError("setupSensors (tempSensor)", errno);
  }
}

void run() {
  printID();
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

void printID() {
  char* buffer;
  int size = sprintf(buffer, "ID=%s\n", id);
  write(1, buffer, size);
  SSL_write(ssl, buffer, size);
}

void modify() {
  char string[256];
  int strSize = SSL_read(ssl, string, 256);
  string[strSize] = '\0';
  
  int start = 0;
  int j;
  for (j = 0; j < strSize; j++) {
    if (string[j] == '\n') {
      string[j] = '\0';

      if (parse(string + start)) {
        write(1, string + start, j - start);
        //write(sockfd, string + start, j - start);
        write(1, "\n", 1);
        //write(sockfd, "\n", 1);
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
    write(1, buffer, 4);
    //write(sockfd, buffer, 5);
    turn_off();
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
  SSL_write(ssl, buffer, bufferSize);
  //fprintf(stderr, "Temperature gotten..\n");
  return;
}

void turn_off() {
  time_t rawTime = time(NULL);
  struct tm * currTime;
  currTime = localtime(&rawTime);

  char buffer[50];
  int bufferSize = sprintf(buffer, "%.2d:%.2d:%.2d SHUTDOWN\n\n", currTime->tm_hour, currTime->tm_min, currTime->tm_sec);
  write(1, buffer, bufferSize - 1);
  SSL_write(ssl, buffer, bufferSize - 1);

  mraa_aio_close(tempSensor);
  close(sockfd);
  SSL_shutdown(ssl);

  _exit(0);
}


int main(int argc, char *argv[]) {
  processArguments(argc, argv);
  createSocket();
  setupPoller();
  setupSensors();
  run();
}
