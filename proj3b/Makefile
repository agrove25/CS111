default:
	echo 'python3.6 ./lab3b.py $$1' > lab3b.sh
	cp lab3b.sh lab3b
	chmod +x lab3b
	rm lab3b.sh

dist: default
	tar -czvf lab3b-304785991.tar.gz Makefile lab3b.py README

clean:
	rm lab3b-304785991.tar.gz lab3b
