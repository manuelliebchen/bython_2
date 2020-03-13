#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct node{
	void* data;
	struct node * next;
};

typedef struct node* llist;

llist llist_push_alloc(void* data, size_t memory, llist next);
llist llist_push(void* data, llist next);
void* llist_peek(llist list);
llist llist_pop(llist list);
bool llist_has_next(llist list);



