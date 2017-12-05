# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

default:
	gcc -lmraa -lm -w -Wall -Wextra -o lab4b lab4b.c

dist: default
	tar -czvf lab4b-304785991.tar.gz Makefile lab4b.c README check.sh

clean:
	rm lab4b-304785991.tar.gz lab4b 
	-rm log.txt

check: default
	chmod +x check.sh
	./check.sh
