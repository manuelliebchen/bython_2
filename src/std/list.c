#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  void *data;
  struct node *next;
};

typedef struct node *List;

bool is_null(void *ptr) { return ptr == NULL; }

bool list_has_next(List list) {
  if (list == NULL) {
    return false;
  }
  return list->next != NULL;
}

List list_push(void *data, List next) {
  List new = (List)malloc(sizeof(struct node));
  new->data = data;
  new->next = next;
  return new;
}

List list_push_alloc(void *data, size_t memory, List next) {
  void *data_alloc = malloc(memory);
  memcpy(data_alloc, data, memory);
  return list_push(data_alloc, next);
}

void *list_peek(List list) {
  if (list == NULL) {
    puts("Error: List is Null, while list_peek!");
    exit(0);
  }
  return list->data;
}

List list_pop(List list) {
  if (list == NULL) {
    return list;
  }
  List next = list->next;
  free(list->data);
  free(list);
  return next;
}

List list_concatenate(List list, List list2) {
  if (list == NULL) {
    return list2;
  }
  List l = list;
  while (l->next != NULL) {
    l = l->next;
  }
  l->next = list2;
  return list;
}

List list_init_main(int32_t argc, char **argv) {
  List list = NULL;
  for (int i = argc - 1; i >= 0; --i) {
    list = list_push_alloc(argv + i, sizeof(argv), list);
  }
  return list;
}

List list_push_int(int32_t a, List next) {
  return list_push_alloc(&a, sizeof(a), next);
}

int32_t list_peek_int(List list) { return *(int32_t *)list_peek(list); }

List list_push_bool(bool a, List next) {
  return list_push_alloc(&a, sizeof(a), next);
}

bool list_peek_bool(List list) { return *(bool *)list_peek(list); }

List list_push_float(float a, List next) {
  return list_push_alloc(&a, sizeof(a), next);
}

float list_peek_float(List list) { return *(float *)list_peek(list); }

List list_push_record(void *a, List next) {
  return list_push_alloc(&a, sizeof(a), next);
}

char *list_peek_record(List list) { return *(void **)list_peek(list); }
