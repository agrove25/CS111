#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

bool triggerSeg = false;

void copy() {
	char buffer[1];
	ssize_t status = read(0, buffer, 1);

	while (status > 0) {
		write(1, buffer, 1);
		status = read(0, buffer, 1);
	}
}

void diagnoseError(int err) {
	switch (err) {
		case 2: fprintf(stderr, "(Invalid file name)"); break;
		case 21: fprintf(stderr, "(File is a directory)"); break;
		case 30: fprintf(stderr, "(File is read-only)"); break;
		default: fprintf(stderr, "(Error Number: %i)", err); break;
	}
}

void sighandler() {
	int err = errno;

	fprintf(stderr, "Encountered a segmentation fault ");

	if (triggerSeg) {
		fprintf(stderr, "due to --segfault ");	
	}

	diagnoseError(err);
	fprintf(stderr, "\n");

	exit(4);
}

int main(int argc, char** argv) {
	char opt;													// the value that will determine which options are used
	char* input = NULL;
	char* output = NULL;
	
	static struct option long_options[] =
	{
		{"input", required_argument, NULL, 'i'},
		{"output", required_argument, NULL, 'o'},
		{"segfault", no_argument, NULL, 's'},
		{"catch", no_argument, NULL, 'c'},
		{0, 0, 0, 0}
	};

	while((opt = getopt_long(argc, argv, "i:o:sc", long_options, NULL)) != -1) {
		switch(opt) {
			case 'i' : input = optarg; break;
			case 'o' : output = optarg; break;
			case 's' : triggerSeg = true; break;
			case 'c' : signal(SIGSEGV, sighandler); break;

			default  : fprintf(stderr, "Usage : lab0 [OPTION] = [ARGUMENT]\n");
			           fprintf(stderr, "OPTION: \n\t--input=filename, \n\t--output=filename, \n\t--segfault, \n\t--catch \n");
			           exit(1);
			           break;

		}
	}

	if (input) {
		int ifd = open(input, O_RDONLY);
		if (ifd >= 0) {
			close(0);
			dup(ifd);
			close(ifd);
		}
		else {
			int err = errno;
			
			fprintf(stderr, "Unable to open file specified in --input: %s ", input);
			diagnoseError(err);
			fprintf(stderr, "\n");
			
			exit(2);
		}
	}

	if (output) {
		int ofd = creat(output, 0666);
		if (ofd >= 0) {
			close(1);
			dup(ofd);
			close(ofd);
		}
		else {
			int err = errno;
			
			fprintf(stderr, "Unable to create file specified in --output: %s ", output);
			diagnoseError(err);
			fprintf(stderr, "\n");
			
			exit(3);
		}
	}

	if (triggerSeg) {
		char *nullptr = NULL;
		char c = *nullptr;
	}

	copy();

	exit(0);
}