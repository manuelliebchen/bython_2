#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define growth_factor 1.5f

struct data {
  void *data;
  size_t refs;
};

size_t count = 0;
size_t capacity = 16;

struct data *data;

void *b_malloc(size_t bytes) {
  if (count + 1 > capacity) {
    capacity *= growth_factor;
    data = realloc(data, capacity * sizeof(struct data));
  }
  for (size_t i = 0; i < count; ++i) {
    if (data[i].refs <= 0) {
      void *memory = malloc(bytes);
      data[i].refs = 1;
      data[i].data = memory;
      return memory;
    }
  }
  void *memory = malloc(bytes);
  data[count].refs = 1;
  data[count].data = memory;
  count++;
  return memory;
}

void b_free(void *memory) {
  for (size_t i = 0; i < count; ++i) {
    if (memory == data[i].data) {
      data[i].refs--;
      if (data[i].refs == 0) {
        free(data[i].data);
        data[i].data = NULL;
      }
      return;
    }
  }
}

void b_init() { data = malloc(capacity * sizeof(struct data)); }

void b_deinit() {
  for (size_t i = 0; i < count; ++i) {
    if (data[i].refs > 0) {
      free(data[i].data);
      data[i].refs = 0;
      data[i].data = NULL;
    }
  }
  free(data);
  printf("Count: %d\n", count);
}

struct node {
  struct node *next;
  void *data;
};

typedef struct node *List;

bool is_null(void *ptr) { return ptr == NULL; }

bool list_has_next(List list) { return list != NULL && list->next != NULL; }

List list_push_alloc(void *data, size_t memory, List next) {
  void *data_alloc = b_malloc(memory + sizeof(struct node));
  memcpy(data_alloc + sizeof(struct node), data, memory);
  List new = data_alloc;
  new->data = data_alloc + sizeof(struct node);
  new->next = next;
  return new;
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
  b_free(list);
  return next;
}

void list_free(List list) {
  while (list != NULL) {
    list = list_pop(list);
  }
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
