default:
	gcc -o lab0 -g lab0.c

check: default runTests
	cat error.log;
	rm -f error.log;

clean: 
	rm -f lab0 *.tar.gz *.txt

dist: default
	tar -czvf lab0-304785991.tar.gz Makefile lab0.c backtrace.png breakpoint.png

runTests:
	echo "Hello World" > sampleInput.txt;
	touch error.log;

	# normal input and output
	./lab0 --input=sampleInput.txt --output=sampleOutput.txt;
	if [ $$? -ne 0 ]; then \
		echo "Standard in/out error." >> error.log; \
	fi;

	diff sampleInput.txt sampleOutput.txt;
	if [ $$? -ne 0 ]; \
	then \
		echo "Incorrect data copied." >> error.log; \
	fi;

	# invalid inputs
	./lab0 --input=none.txt 2> /dev/null ; \
	if [ $$? -ne 2 ] ; \
	then \
		echo "Incorrect invalid input handling." >> error.log; \
	fi;

	# invalid outputs
	./lab0 --output="jfiow\/\/we\fwe/.txt" 2> /dev/null ; \
	if [ $$? -ne 3 ] ; \
	then \
		echo "Incorrect invalid output handling." >> error.log; \
	fi;

	# segmentation and signal handler
	./lab0 --segfault --catch 2> /dev/null ; \
	if [ $$? -ne 4 ] ; \
	then \
		echo "Incorrect signal handling." >> error.log; \
	fi;

	# invalid arguments
	./lab0 --feiow 2> /dev/null ; \
	if [ $$? -ne 1 ] ; \
	then \
		echo "Incorrect invalid arguments handling." >> error.log; \
	fi;

	rm -f sampleInput.txt sampleOutput.txt;