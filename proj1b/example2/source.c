#include <stdio.h>
#include <unistd.h>

// cryption
#include <mcrypt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

MCRYPT td;

void prepareEncryption() {
  char* key;
  char* IV;
  int keysize = 16;
  key = calloc(1, keysize);

  int keyfd = open("my.key", O_RDONLY);
  if (keyfd < 0) {
    
  }

  read(keyfd, key, keysize);
  td = mcrypt_module_open("twofish", NULL, "cfb", NULL);
  if (td == MCRYPT_FAILED) {
    
  }

  IV = (char*) malloc(mcrypt_enc_get_iv_size(td));
  if (IV == NULL) {
    
  }
  for (int i = 0; i < mcrypt_enc_get_iv_size(td); i++) {
    IV[i] = rand();
  }

  int status = mcrypt_generic_init(td, key, keysize, NULL);
  if (status < 0) {
   
  }

  free(key);
}

int main() {
	void prepareEncryption();	

	while (1) {
		char c[1];
		c[0] = getchar();

		fprintf(stderr, "ENTERED: ");
    fprintf(stderr, c);
    fprintf(stderr, "\n");

    if (mcrypt_generic(td, &c[0], 1) < 0) {
      fprintf(stderr, "error at mcrypt");
    }

    fprintf(stderr, "ENCRYPT: ");
    fprintf(stderr, c);
    fprintf(stderr, "\n");
    
    if (mdecrypt_generic(td, &c[0], 1) < 0) {
      
    }

		putchar(c[0]);
	}


}