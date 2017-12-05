default:
	gcc -g -lmraa -lm -w -Wall -Wextra -o lab4c_tcp lab4c_tcp.c
	gcc -g -lmraa -lm -w -Wall -Wextra -o lab4c_tls lab4c_tls.c -lssl -lcrypto 

dist: default
	tar -czvf lab4c-304785991.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README

clean:
	rm lab4c_tcp lab4c_tls lab4c-304785991.tar.gz
