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
#include <signal.h>

int n_threads = 1;
int n_iterations = 1;
int n_lists = 1;
int opt_yield = 0;
char opt_sync = 'n';
int exitCode = 0;

SortedList_t* lists;
SortedListElement_t* elements;
int* sublists;

pthread_mutex_t* mutex;
int* exclusion;

long long waitTime = 0;

void operate();

void handleError(char loc[256], int err) {
  fprintf(stderr, "Error encountered in ");
  fprintf(stderr, loc);
  fprintf(stderr, ": ");
  fprintf(stderr, strerror(err));

  exit(1);
}

void signal_handler() {
  fprintf(stderr, "Encountered Segmentation Fault.\n");
  exit(2);
}

void processArguments(int argc, char* argv[]) {
  char opt;

  static struct option long_options[] =
  {
    {"threads", required_argument, NULL, 't'},
    {"iterations", required_argument, NULL, 'i'},
    {"yield", required_argument, NULL, 'y'},
    {"sync", required_argument, NULL, 's'},
    {"lists", required_argument, NULL, 'l'},
    {0, 0, 0, 0}
  };

  while ( (opt = getopt_long(argc, argv, "t:i:y:s:l:", long_options, NULL)) != -1 ) {
    switch(opt) {
        case 't': n_threads = atoi(optarg); break;
        case 'i': n_iterations = atoi(optarg); break;
        case 'y': setYieldModes(optarg); break;
        case 's': opt_sync = optarg[0]; break;
        case 'l': n_lists = atoi(optarg); break;

        default:  fprintf(stderr, "Usage : server [OPTION] = [ARGUMENT]\n");
  			          fprintf(stderr, "OPTION: \n\t--threads=numthreads\
                                  \n\t--iterations=numiterations\
                                  \n\t--yield=yieldModes\
                                  \n\t--sync=syncOption\n");
  			          exit(1);
  			          break;

    }
  }

  initialize_syncs();
  signal(SIGSEGV, signal_handler);
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

void initialize_syncs() {
  if (opt_sync == 'm') {
    mutex = malloc(n_lists * sizeof(pthread_mutex_t));
    if (mutex == NULL) {
      handleError("initialize_syncs (creating mutexes)", errno);
    }

    for (int i = 0; i < n_lists; i++) {
      pthread_mutex_init(&mutex[i], NULL);
    }
  }
  else if (opt_sync == 's') {
    exclusion = malloc(n_lists * sizeof(int));
    if (exclusion == NULL) {
      handleError("initialize_syncs (creating exclusions)", errno);
    }

    for (int i = 0; i < n_lists; i++) {
      exclusion[i] = 0;
    }
  }
}

void setupElements() {
  lists = malloc(sizeof(SortedList_t) * n_lists);
  for (int i = 0; i < n_lists; i++) {
    lists[i].key = "null";
    lists[i].next = &lists[i];
    lists[i].prev = &lists[i];
  }

  elements = malloc((n_iterations + 1) * sizeof(SortedListElement_t));
  if (elements == NULL) handleError("setupElments (init elements)", errno);

  sublists = malloc((n_iterations + 1) * sizeof(int));
  if (sublists == NULL) handleError("setupElments (init sublists)", errno);

  srand(time(NULL));
  for (int i = 0; i < n_iterations; i++) {
    char *key = malloc(6 * sizeof(char));

    for (int j = 0; j < 5; j++) {
      key[j] = rand() % 26 + 'a';
    }
    key[5] = '\0';
    elements[i].key = key;
    sublists[i] = hash_function(elements[i].key) % n_lists;
  }

}

void runThreads() {
  pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
  int thread_id[n_threads];
  for (int i = 0; i < n_threads; i++) {
    thread_id[i] = i;
  }

  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
    handleError("runThreads (init start clock)", errno);
  }


  for (int i = 0; i < n_threads; i++) {
    if (pthread_create(threads + i, NULL, (void *) operate,
                       thread_id + i) < 0) {
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

  int length = 0;
  for (int i = 0; i < n_lists; i++) {
    int sub_length = SortedList_length(lists + i);
    if (sub_length != 0) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }

    length += sub_length;
  }

  long long totTime = 1000000000 * (end.tv_sec - start.tv_sec)
    + (end.tv_nsec - start.tv_nsec);

  char description[50] = "list-";
  if (opt_yield == 0) sprintf(description, "%s%s", description, "none");
  else {
    if (opt_yield & 0x01) sprintf(description, "%s%c", description, 'i');
    if (opt_yield & 0x02) sprintf(description, "%s%c", description, 'd');
    if (opt_yield & 0x04) sprintf(description, "%s%c", description, 'l');
  }
  sprintf(description, "%s%c", description, '-');

  if (opt_sync == 'n') sprintf(description, "%s%s", description, "none");
  else sprintf(description, "%s%c", description, opt_sync);

  long long ops = n_threads * n_iterations * 3;
  long long avgTime = totTime / ops;

  if (opt_sync == 'n') waitTime = 0;
  long long avgWaitTime = waitTime / ops;

  printf("%s,%d,%d,%d,%d,%d,%d,%d\n", description, n_threads, n_iterations,
          n_lists, ops, totTime, avgTime, avgWaitTime);

  free(threads);
}

void operate(void* id_arg) {
  int* id = (int*) id_arg;

  for (int i = *id; i < n_iterations; i += n_threads) {
    lock(sublists[i]);
    SortedList_insert(&(lists[sublists[i]]), &(elements[i]));
    unlock(sublists[i]);
  }

  int length = 0;
  for (int i = 0; i < n_lists; i++) {
    lock(i);
    int sub_length = SortedList_length(lists + i);
    if (sub_length == -1) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }

    length += sub_length;
    unlock(i);
  }

  SortedListElement_t* temp;
  for (int i = *id; i < n_iterations; i += n_threads) {
    lock(sublists[i]);

    temp = SortedList_lookup(lists + sublists[i], elements[i].key);
    if (temp == NULL) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }

    if (SortedList_delete(temp) == 1) {
      fprintf(stderr, "invalidated List\n");
      exit(2);
    }

    unlock(sublists[i]);
  }
}

void lock(int id) {
  struct timespec start, end;

  if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
    handleError("runThreads (init start clock)", errno);
  }

  switch(opt_sync) {
    case 'n': break;
    case 'm': pthread_mutex_lock(&(mutex[id]));
              break;

    case 's': while(__sync_lock_test_and_set(&exclusion[id], 1));
              break;
  }

  if (clock_gettime(CLOCK_MONOTONIC, &end) < 0) {
    handleError("runThreads (init end clock)", errno);
  }

  waitTime += 1000000000 * (end.tv_sec - start.tv_sec)
    + (end.tv_nsec - start.tv_nsec);
}

void unlock(int id) {
  switch(opt_sync) {
    case 'n': break;
    case 'm': pthread_mutex_unlock(&mutex[id]);
              break;

    case 's': __sync_lock_release(exclusion + id);
              break;
  }
}

/* this hashing function was taken from stackOverflow, which
    was taken from djb2 */
int hash_function(const char* key) {
  unsigned long hash = 5381;
  int c;

  for (int i = 0; i < 5; i++) {
    c = key[i];
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

int main(int argc, char* argv[]) {
  processArguments(argc, argv);
  setupElements();
  runThreads();

  for (int i = 0; i < n_iterations; i++)
    free(elements[i].key);
  free(elements);
  free(lists);
  free(sublists);
  free(mutex);
  free(exclusion);

  exit(exitCode);
}
