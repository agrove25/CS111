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
#include "SortedList.h"

int n_threads = 1;
int n_iterations = 1;
int opt_yield = 0;
char opt_sync = 'n';
int exitCode = 0;

SortedList_t* list;
SortedListElement_t* elements;

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
    {"yield", required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "t:i:y:s:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 't': n_threads = atoi(optarg); break;
        case 'i': n_iterations = atoi(optarg); break;
        case 'y': setYieldModes(optarg); break;
        case 's': opt_sync = optarg[0]; break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--threads=numthreads\
                                  \n\t--iterations=numiterations\
                                  \n\t--yield=yieldModes\
                                  \n\t--sync=syncOption\n");
  			          exit(1);
  			          break;

    }
  }

  switch(opt_sync) {
    case 'n': break;
    case 'm': pthread_mutex_init(&mutex, NULL); break;
    case 's': break;
    default: handleError("processArguments (invalid sync option)", errno);
  }
}

void setYieldModes(char* modes) {
  for (int i = 0; i < strlen(modes); i++) {
    switch(modes[i]) {
      case 'i': opt_yield |= INSERT_YIELD; break;
      case 'd': opt_yield |= DELETE_YIELD; break;
      case 'l': opt_yield |= LOOKUP_YIELD; break;
      default: handleError("setYieldModes (invalid yield mode)", errno);
    }
  }
}

void setupElements() {
  list = malloc(sizeof(SortedList_t));
  if (list == NULL) handleError("setupElments (init list)", errno);

  list->key = NULL;
  list->next = list;
  list->prev = list;

  elements = malloc((n_iterations + 1) * sizeof(SortedListElement_t));
  if (elements == NULL) handleError("setupElments (init elements)", errno);

  srand(time(NULL));
  char key[6];
  for (int i = 0; i < n_iterations; i++) {
    for (int j = 0; j < 5; j++) {
      key[j] = rand() % 26 + 'a';
    }
    key[5] = '\0';
    elements[i].key = key;
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

  if (SortedList_length(list) != 0) {
    fprintf(stderr, "invalidated List\n");
    exit(2);
  }

  long long totTime = 1000000000 * (end.tv_sec - start.tv_sec)
    + (end.tv_nsec - start.tv_nsec);

  char description[50] = "list-";
  if (opt_yield == 0) sprintf(description, "%s%s", description, "none-");
  else {
    if (opt_yield & 0x01) sprintf(description, "%s%c", description, 'i');
    if (opt_yield & 0x02) sprintf(description, "%s%c", description, 'd');
    if (opt_yield & 0x04) sprintf(description, "%s%c", description, 'l');
  }

  if (opt_sync == 'n') sprintf(description, "%s%s", description, "none");
  else sprintf(description, "%s%c", description, opt_sync);

  int ops = n_threads * n_iterations * 3;
  double avgTime = totTime / (double) ops;

  printf("%s,%d,%d,%d,%d,%f,%d\n", description, n_threads, n_iterations,
          ops, totTime, avgTime, 0);

  free(threads);
}

void operate() {
  switch(opt_sync) {
    case 'n': simulate();
              break;

    case 'm': pthread_mutex_lock(&mutex);
              simulate();
              pthread_mutex_unlock(&mutex);
              break;

    case 's': while(__sync_lock_test_and_set(&exclusion, 1));
              simulate();
              __sync_lock_release(&exclusion);
              break;
  }
}

void simulate() {
  for (int i = 0; i < n_iterations; i++) {
    SortedList_insert(list, elements+i) ;
  }

  if (SortedList_length(list) == -1) {
    fprintf(stderr, "invalidated List\n");
    exit(2);
  }

  for (int i = 0; i < n_iterations; i++) {
    if (SortedList_lookup(list, elements[i].key) == NULL) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }
  }

  for (int i = 0; i < n_iterations; i++) {
    if (SortedList_delete(elements+i) == 1) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }
  }
}


int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  setupElements();
  runThreads();

  exit(exitCode);
}
