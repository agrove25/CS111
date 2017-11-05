// NAME: Andrew Grove
// EMAIL: koeswantoandrew@gmail.com
// ID: 304785991

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <getopt.h>
#include <errno.h>

#include <time.h>
#include <pthread.h>

int n_threads = 1;
int n_iterations = 1;
bool opt_yield = false;
char opt_sync = 'n';
int exitCode = 0;

long long counter = 0;

pthread_mutex_t mutex;
int exclusion;

void operate();

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

void processArguments(int argc, char* argv[]) {
  char opt;

  static struct option long_options[] =
  {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", no_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "t:i:ys:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 't': n_threads = atoi(optarg); break;
        case 'i': n_iterations = atoi(optarg); break;
        case 'y': opt_yield = true; break;
        case 's': opt_sync = optarg[0]; break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--threads=numthreads\
                                  \n\t--iterations=numiterations\
                                  \n\t--yield\
                                  \n\t--sync=syncOption\n");
  			          exit(1);
  			          break;

    }
  }

  switch(opt_sync) {
    case 'n': break;
    case 'm': pthread_mutex_init(&mutex, NULL); break;
    case 's': break;
    case 'c': break;
    default: handleError("processArguments (invalid sync option)", errno);
  }
}

void runThreads() {
  pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
    handleError("runThreads (init start clock)", errno);
  }

  for (int i = 0; i < n_threads; i++) {
    if (pthread_create(threads + i, NULL, (void *) operate, NULL) < 0) {
      handleError("runThreads (init threads)", errno);
    }
  }

  for (int i = 0; i < n_threads; i++) {
    if (pthread_join(threads[i], NULL) < 0) {
      handleError("runThreads (join threads)", errno);
    }
  }

  if (clock_gettime(CLOCK_MONOTONIC, &end) < 0) {
    handleError("runThreads (init end clock)", errno);
  }

  char description[20] = "add-";
  if (opt_yield) sprintf(description, "%s%s", description, "yield-");
  if (opt_sync == 'n') sprintf(description, "%s%s", description, "none");
  else sprintf(description, "%s%c", description, opt_sync);

  int ops = n_threads * n_iterations * 2;
  long long totTime = 1000000000 * (end.tv_sec - start.tv_sec)
    + (end.tv_nsec - start.tv_nsec);
  double avgTime = totTime / (double) ops;

  printf("%s,%d,%d,%d,%d,%f,%d\n", description, n_threads, n_iterations,
          ops, totTime, avgTime, 0);

  free(threads);
}

void operate() {
  for (int i = 0; i < n_iterations; i++) {
    switch(opt_sync) {
      case 'n': add(&counter, 1);
                break;

      case 'm': pthread_mutex_lock(&mutex);
                add(&counter, 1);
                pthread_mutex_unlock(&mutex);
                break;

      case 's': while(__sync_lock_test_and_set(&exclusion, 1));
                add(&counter, 1);
                __sync_lock_release(&exclusion);
                break;

      case 'c': add_CAS(1);
                break;
    }

    switch(opt_sync) {
      case 'n': add(&counter, -1);
                break;

      case 'm': pthread_mutex_lock(&mutex);
                add(&counter, -1);
                pthread_mutex_unlock(&mutex);
                break;

      case 's': while(__sync_lock_test_and_set(&exclusion, 1));
                add(&counter, -1);
                __sync_lock_release(&exclusion);
                break;

      case 'c': add_CAS(-1);
                break;
    }
  }
}

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (opt_yield)
    sched_yield();
  *pointer = sum;
}

void add_CAS(long long value) {
  long long old, new;

  while (1) {
    old = counter;
    new = old + value;

    if (opt_yield)
      sched_yield();

    if (__sync_val_compare_and_swap(&counter, old, new) == old)
      break;
  }
}

int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  runThreads();
  exit(0);
}
