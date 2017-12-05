#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>

int sockfd, n;
int port = 19000;
char* host = "lever.cs.ucla.edu";

struct hostent *server;
struct sockaddr_in server_in;
SSL* ssl;


void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));
  fprintf(stderr, "\n");

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

  if (SSL_write(ssl, "ID=100110011\n", 13) < 0) {
    int err = errno;
    handleError("ssl_write id", err);
  }
  for (int i = 0; i < 10; i++) {
    SSL_write(ssl, "Hello\n", 6);
  }

  fprintf(stderr, "Messages have been sent.\n");
}
