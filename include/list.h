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
#define LIST_ACCESS_IT(LIST, TYPE) (TYPE*)list_access_it(LIST)

void list_make(list_t* list, usize elem_size);

void list_append(list_t* list, void* elem);
void* list_access(list_t* list, usize i);
ssize list_search(list_t* list, void* elem);

void list_delete(list_t* list, usize i);
void list_delete_all(list_t* list);

static inline usize list_size(const list_t* list) {
	return list->size; 
}

static inline void* list_access_it(list_t* list) {
	return list->it->elem;
}

static inline void list_reset_it(list_t* list) {
	list->it = list->head;
}

static inline void list_increment_it(list_t* list) {
	list->it = list->it->next;
}
