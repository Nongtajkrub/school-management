#pragma once

#include "type.h"

typedef struct node {
	void* elem;

	struct node* next;
} node_t;

typedef struct {
	usize size;
	usize elem_size;

	node_t* head;
	node_t* it;
} list_t;

#define LIST_MAKE(LIST, TYPE) list_make(LIST, sizeof(TYPE))
#define LIST_APPEND(LIST, TYPE, ELEM)                                         \
	do {                                                                      \
		TYPE elem = ELEM;                                                     \
		list_append(LIST, (void*)&elem);                                      \
	} while (0)
#define LIST_ACCESS(LIST, TYPE, I) (TYPE*)list_access(LIST, I)
#define LIST_TAIL(LIST, TYPE) (TYPE*)list_access(LIST, list_size(LIST) - 1)
#define LIST_IT_ACCESS(LIST, TYPE) (TYPE*)list_it_access(LIST)

void list_make(list_t* list, usize elem_size);

void list_append(list_t* list, void* elem);
// list_append but only allocate space not copy new element into the list
void list_append_none(list_t* list);
void* list_access(list_t* list, usize i);
ssize list_search(list_t* list, void* elem);

void list_delete(list_t* list, usize i);
void list_delete_all(list_t* list);

static inline usize list_size(const list_t* list) {
	return list->size; 
}

static inline void* list_it_access(list_t* list) {
	return list->it->elem;
}

static inline void list_it_reset(list_t* list) {
	list->it = list->head;
}

static inline void list_it_increment(list_t* list) {
	list->it = list->it->next;
}
