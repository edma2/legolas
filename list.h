/* A singly-linked list. */
struct List {
  void *data;
  struct List *next;
};
typedef struct List List;

void *List_append(List **npp, void *data);
void List_free(List **npp, void (*free_data)(void *));

void List_test(void);
