#include "SortedList.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
  if (list == NULL || element == NULL) {
    return;
  }

  int count = 0;
  SortedListElement_t *curr = list->next;

  while (curr != list && curr->next != curr) {
    if (count > 1000000) {
      return;
    }
    if (strcmp(element->key, curr->key) < 0) {
      break;
    }

    count++;
    curr = curr->next;
  }

  if (opt_yield & INSERT_YIELD) {
    pthread_yield();
  }


  element->prev = curr->prev;
  element->next = curr;
  curr->prev->next = element;
  curr->prev = element;
}

int SortedList_delete( SortedListElement_t *element) {
  if (element == NULL)
    return 1;

  if (opt_yield & DELETE_YIELD) {
    pthread_yield();
  }

  if (element->prev->next == element->next->prev) {
    element->prev->next = element->next;
    element->next->prev = element->prev;
    return 0;
  }

  return 1;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  if (list == NULL || key == NULL)
    return NULL;

  int count = 0;
  SortedListElement_t *curr = list->next;

  while (curr != list) {
    if (count > 1000000) return NULL;
    if (strcmp(curr->key, key) == 0) {
      return curr;
    }

    if (opt_yield & LOOKUP_YIELD) {
      pthread_yield();
    }

    count++;
    curr = curr->next;
  }

  return NULL;
}

int SortedList_length(SortedList_t *list) {
  if (list == NULL)
    return -1;

  SortedListElement_t *curr = list->next;
  int count = 0;

  while (curr != list) {
    if (count > 1000000) return -1;

    count++;
    if (opt_yield & LOOKUP_YIELD) {
      pthread_yield();
    }

    curr = curr->next;
  }

  return count;
}
