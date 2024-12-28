#pragma once

#include "type.h"

#include <memory.h>
#include <stdlib.h>

#define ARRAY_MAKE(ARR, TYPE, SIZE) array_make(ARR, SIZE, sizeof(TYPE))
#define ARRAY_GET(ARR, TYPE, I) (TYPE*)array_get(ARR, I) 
#define ARRAY_SET(ARR, TYPE, ELEM, I) \
	do {\
		TYPE elem = ELEM;\
		array_set(ARR, I, &elem);\
	} while(0)

typedef struct {
	// metadata
	usize size;
	usize elem_size;

	// array data
	void** elem;
} array_t;

void array_make(array_t* arr, usize size, usize elem_size);

static inline void* array_get(array_t* arr, usize i) {
	return arr->elem[i];
}

static inline void array_set(array_t* arr, usize i, void* elem) {
	memcpy(arr->elem[i], elem, arr->elem_size);
}

static inline usize array_size(array_t* arr) {
	return arr->size;
}
