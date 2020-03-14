#include "../std/list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llist llist_push_alloc(void *data, size_t memory, llist next) {
  void *data_alloc = malloc(memory);
  memcpy(data_alloc, data, memory);
  return llist_push(data_alloc, next);
}

llist llist_push(void *data, llist next) {
  llist new = (llist)malloc(sizeof(struct node));
  new->data = data;
  new->next = next;
  return new;
}

void *llist_peek(llist list) { return list->data; }

bool llist_has_next(llist list) { return list->next != NULL; }

llist llist_pop(llist list) {
  llist next = list->next;
  free(list->data);
  free(list);
  return next;
}

llist llist_init_main(int32_t argc, char **argv) {
  llist list = NULL;
  for (int i = argc - 1; i >= 0; --i) {
    list = llist_push_alloc(argv + i, sizeof(argv), list);
  }
  return list;
}
