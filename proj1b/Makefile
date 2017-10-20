# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

default:
	gcc -w -Wall -Wextra -o lab1b-server server.c -lmcrypt
	gcc -w -Wall -Wextra -o lab1b-client client.c -lmcrypt

clean:
	rm -f lab1b-client lab1b-server *.tar.gz

dist: default
	tar -czvf lab1b-304785991.tar.gz Makefile client.c server.c \
	my.key README
