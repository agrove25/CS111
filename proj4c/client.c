#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

int sockfd, n;
int port = 18000;
char* host = "lever.cs.ucla.edu";

struct hostent *server;
struct sockaddr_in server_in;


void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

int main() {
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


  write(sockfd, "ID=100110011\n", 13);
  int i;
  for (i = 0; i < 10; i++) {
    write(sockfd, "Hello\n", 6);
  }

  close(sockfd);
  exit(0);

}
