
#include "list.h"

#include <memory.h>
#include <stdlib.h>

#define NODE_SIZE sizeof(node_t)

void list_make(list_t* list, usize elem_size) {
	list->size = 0;
	list->elem_size = elem_size;

	list->head = malloc(NODE_SIZE);
	ASSERT(list->head != NULL, DEF_ALLOC_ERRMSG);
	list->head->elem = NULL;
	list->head->next = NULL;

	list->it = list->head;
}

// return last node
static node_t* traverse_till_tail(list_t* list) {
	node_t* node = list->head;

	while (node->next != NULL) {
		node = node->next;
		printf("finding\n");
	}

	printf("found\n");
	return node;
}

static node_t* traverse_till_index(list_t* list, usize i) {
	if (i == 0) {
		return list->head;
	}

	node_t* node = list->head;

	for (usize j = 0; j < i; j++) {
		node = node->next;
	}

	return node;
}

void list_append(list_t* list, void* elem) {
	node_t* tail = traverse_till_tail(list);

	// init new node
	tail->elem = malloc(list->elem_size);
	ASSERT(tail->elem != NULL, DEF_ALLOC_ERRMSG);
	memcpy(tail->elem, elem, list->elem_size);

	tail->next = malloc(NODE_SIZE);
	tail->next->next = NULL;
	ASSERT(tail->next != NULL, DEF_ALLOC_ERRMSG);

	list->size++;
}

void* list_access(list_t* list, usize i) {
	return (list_size(list) > 0 && i < list->size) ?
		(traverse_till_index(list, i))->elem : NULL;
}

ssize list_search(list_t* list, void* elem) {
	node_t* node = list->head;
	usize i = 0;

	while (true) {
		if (memcmp(node->elem, elem, list->elem_size) == 0) {
			break;
		}
		if (node->next == NULL) {
			return -1;
		}
		node = node->next;
		i++;
	}

	return i;
}

static void handle_delete_head(list_t* list) {
	node_t* old_head = list->head;

	list->head = list->head->next;

	free(old_head);
	list->size--;
}

static void handle_delete_tail(list_t* list) {
	node_t* node_before_tail = traverse_till_index(list, list->size - 2);
	node_t* tail = node_before_tail->next;

	free(tail->elem);
	free(tail);
	node_before_tail->next = NULL;
	list->size--;
}

void list_delete(list_t* list, usize i) {
	if (list->size == 0) {
		return;
	}

	// trying to delete head
	if (i == 0) {
		handle_delete_head(list);
		return;
	}
	// trying to delete tail
	if (i == list->size - 1) {
		handle_delete_tail(list);
		return;
	}

	// delete at a position that is not head nor tail
	node_t* prev_node = traverse_till_index(list, i - 1);
	node_t* this_node = prev_node->next;

	prev_node->next = this_node->next;

	free(this_node->elem);
	free(this_node);
	list->size--;
}

void list_delete_all(list_t* list) {
	// delete till only the head is left
	while (list->size != 1) {
		list_delete(list, 0);
		list->size--;
	}

	free(list->head->elem);
	free(list->head);
}
