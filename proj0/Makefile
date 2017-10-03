default:
	gcc -o lab0 -g lab0.c

check: default runTests

clean: 
	rm -f lab0 *.tar.gz *.txt

dist: default
	tar -czvf lab0-304785991.tar.gz Makefile lab0.c

runTests:
	echo "Hello World" > sampleInput.txt;

	# normal input and output
	./lab0 --input=sampleInput.txt --output=sampleOutput.txt;
	if [ $$? -ne 0 ]; then \
		echo "Standard in/out error."; \
	fi;

	diff sampleInput.txt sampleOutput.txt;
	if [ $$? -ne 0 ]; \
	then \
		echo "Incorrect data copied."; \
	fi;

	# incorrect input
	./lab0 --input=incorrectFile.txt 2> /dev/null;
	if [ $$? -eq 2 ]; then \
		echo "Incorrect handling of invalid input"; \
	fi;

	# incorrect output
	./lab0 --output="fie\/3jh2\./";
	if [ $$? -eq 3 ]; then \
		echo "Incorrect handling of invalid output"; \
	fi;






