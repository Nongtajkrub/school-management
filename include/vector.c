#include "vector.h"

#include <stdlib.h>
#include <memory.h>

#define DEF_VEC_CAP 4

void vec_make(vec_t* vec, usize elem_size) {
	vec->size = 0;
	vec->capacity = DEF_VEC_CAP;
	vec->elem_size = elem_size;
	vec->elem = calloc(DEF_VEC_CAP, sizeof(void*));
	ASSERT(vec->elem != NULL, DEF_ALLOC_ERRMSG);
}

void vec_destroy(vec_t* vec) {
	for (usize i = 0; i < vec_size(vec); i++) {
		free(vec->elem[i]);
	}
	free(vec->elem);

	vec->capacity = DEF_VEC_CAP;
	vec->size = 0;
	vec->elem_size = 0;
}

void vec_push(vec_t* vec, void* elem) {
	if (vec->size >= vec->capacity) {
		vec->capacity *= 2;
		vec->elem = realloc(vec->elem, vec->capacity * sizeof(void*));
		ASSERT(vec->elem != NULL, DEF_ALLOC_ERRMSG);
	}
	vec->elem[vec->size] = malloc(vec->elem_size);
	ASSERT(vec->elem[vec->size], DEF_ALLOC_ERRMSG);
	memcpy(vec->elem[vec->size], elem, vec->elem_size);
	vec->size++;
}

void vec_pop(vec_t* vec) {
	if (vec->size == 0) {
		return;
	}
	free(vec_back(vec));
	vec->size--;
}

void vec_clear(vec_t* vec) {
	for (usize i = 0; i < vec->size; i++) {
		free(vec->elem[i]);
	}
	free(vec->elem);

	vec_make(vec, vec->elem_size);
}
