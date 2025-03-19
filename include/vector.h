#pragma once

#include "type.h"

#ifndef VEC_STARTING_CAPACITY
	#define VEC_STARTING_CAPACITY 4
#endif // #ifndef VEC_STARTING_CAPACITY

#define VEC_MAKE(VEC, TYPE) vec_make(VEC, sizeof(TYPE))
#define VEC_PUSH(VEC, TYPE, ELEM)                                             \
	do {                                                                      \
		TYPE elem = ELEM;                                                     \
		vec_push(VEC, &elem);                                                 \
	} while(0)
#define VEC_GET(VEC, TYPE, I) (TYPE*)vec_get(VEC, I)
#define VEC_BACK(VEC, TYPE) (TYPE*)vec_back(VEC)

typedef struct {
	usize capacity;
	usize size;

	usize elem_size;
	void** elem;
} vec_t;

void vec_make(vec_t* vec, usize elem_size);
vec_t vec_new(usize elem_size);
void vec_destroy(vec_t* vec);

void vec_push(vec_t* vec, void* elem);
// vec_push but only allocate space not copy new element into the vector
void vec_push_none(vec_t* vec);
void* vec_pop(vec_t* vec);
void vec_pop_back(vec_t* vec);
void vec_swapback(vec_t* vec, u32 i);

void* vec_get(vec_t* vec, u32 i);

void vec_clear(vec_t* vec);

static inline void* vec_back(vec_t* vec) {
	return (vec->size == 0) ? NULL : vec->elem[vec->size - 1];
}

static inline usize vec_size(vec_t* vec) {
	return vec->size;
}

static inline usize vec_mem_size(vec_t* vec) {
	return vec->size * vec->elem_size;
}

static inline bool vec_empty(vec_t* vec) {
	return (vec->size == 0);
}
