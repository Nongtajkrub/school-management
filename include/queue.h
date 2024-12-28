/*
 * queue is really just an alias for link list
*/

#pragma once

#include "type.h"
#include "list.h"

typedef list_t queue_t; 

#define QUEUE_MAKE(QUEUE, TYPE) LIST_MAKE(QUEUE, TYPE)
#define QUEUE_APPEND(QUEUE, TYPE, ELEM) LIST_APPEND(QUEUE, TYPE, ELEM)
#define QUEUE_POP(QUEUE, TYPE) LIST_ACCESS(QUEUE, TYPE, 0)

static inline void queue_make(queue_t* queue, usize elem_size) {
	list_make(queue, elem_size);
}

static inline void queue_append(queue_t* queue, void* elem) {
	list_append(queue, elem);
}

static inline void* queue_pop(queue_t* queue) {
	return (list_size(queue) > 0) ? list_access(queue, 0) : NULL;
} 

static inline usize queue_size(queue_t* queue) {
	return list_size(queue);
}
