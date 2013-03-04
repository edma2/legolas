#include <assert.h>
#include <list.h>
#include <stdlib.h>
#include <stdio.h>

void *List_append(List **npp, void *data) {
  List *np, *np2;

  np = malloc(sizeof(List));
  if (np == NULL) {
    return NULL;
  }
  np->data = data;
  np->next = NULL;

  // empty list
  if (*npp == NULL) {
    *npp = np;
  } else {
    for (np2 = *npp; np2->next != NULL; np2 = np2->next)
      ;
    np2->next = np;
  }

  return np;
}

/* If free_data is non-NULL then call free_data(np->data) */
void List_free(List **npp, void (*free_data)(void *)) {
  List *np, *next;

  for (np = *npp; np != NULL; np = next) {
    if (free_data != NULL) {
      free_data(np->data);
    }
    next = np->next;
    free(np);
  }
}

void List_test(void) {
  List *np, *head = NULL;
  int i;

  for (i = 0; i < 10; i++) {
    List_append(&head, (void *)i);
  }
  np = head;
  for (i = 0; i < 10; i++) {
    assert((int)np->data == i);
    printf("%d\n", i);
    np = np->next;
  }

  List_free(&head, NULL);
  // make sure free empty list doesn't choke
  head = NULL;
  List_free(&head, NULL);
}
