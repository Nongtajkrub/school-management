#include "vector.h"
#include "err_msg.h"

#include <memory.h>

void vec_make(vec_t* vec, usize elem_size) {
	vec->capacity = VEC_STARTING_CAPACITY;
	vec->size = 0;
	vec->elem_size = elem_size;

	vec->elem = calloc(vec->capacity, sizeof(void*));
	ASSERT(vec->elem != NULL, DEF_ALLOC_ERRMSG);
}

vec_t vec_new(usize elem_size) {
	vec_t vec = {
		.capacity = VEC_STARTING_CAPACITY,
		.size = 0,
		.elem_size = elem_size,
		.elem = calloc(VEC_STARTING_CAPACITY, sizeof(void*))
	};

	ASSERT(vec.elem != NULL, DEF_ALLOC_ERRMSG);
	return vec;
}

void vec_destroy(vec_t* vec) {
	for (usize i = 0; i < vec->size; i++) {
		free(vec->elem[i]);
	}
	free(vec->elem);

	vec->capacity = 0;
	vec->size = 0;
	vec->elem_size = 0;
}

static void resize(vec_t* vec, usize size) {
	if (size < VEC_STARTING_CAPACITY) {
		size = VEC_STARTING_CAPACITY;
	}

	if (vec->capacity == size) {
		return;
	}

	vec->capacity = size;
	vec->elem = realloc(vec->elem, vec->capacity * sizeof(void*));
	ASSERT(vec->elem != NULL, DEF_ALLOC_ERRMSG);
}

void vec_push(vec_t* vec, void* elem) {
	if (vec->size == vec->capacity) {
		resize(vec, vec->capacity * 2);
	}

	vec->elem[vec->size] = malloc(vec->elem_size);
	ASSERT(vec->elem[vec->size] != NULL, DEF_ALLOC_ERRMSG);
	memcpy(vec->elem[vec->size], elem, vec->elem_size);
	vec->size++;
}

static inline void shrink_if_needed(vec_t* vec) {
	if (vec->size < vec->capacity / 4) {
		resize(vec, vec->capacity / 2);
	}
}

void* vec_pop(vec_t* vec) {
	ASSERT(vec->size != 0, VEC_UNDERFLOW_ERRMSG);

	vec->size--;
	void* pop_elem = vec->elem[vec->size];
	vec->elem[vec->size] = NULL;

	shrink_if_needed(vec);
	return pop_elem;
}

void vec_pop_back(vec_t* vec) {
	ASSERT(vec->size != 0, VEC_UNDERFLOW_ERRMSG);

	vec->size--;
	free(vec->elem[vec->size]);
	vec->elem[vec->size] = NULL;

	shrink_if_needed(vec);
}

void vec_swapback(vec_t* vec, u32 i) {
	ASSERT(vec->size != 0, VEC_UNDERFLOW_ERRMSG);
	ASSERT(i < vec->size, VEC_OUT_OF_BOUND_ERRMSG);

	void* back = vec_back(vec);

	memcpy(vec->elem[i], back, vec->elem_size);
	free(back);
	vec->size--;

	shrink_if_needed(vec);
}

void* vec_get(vec_t* vec, u32 i) {
	ASSERT(i < vec->size, VEC_OUT_OF_BOUND_ERRMSG);
	return vec->elem[i];
}

void vec_clear(vec_t* vec) {
	for (usize i = 0; i < vec->size; i++) {
		free(vec->elem[i]);
	}

	vec->size = 0;
	resize(vec, VEC_STARTING_CAPACITY);
}
