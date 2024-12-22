#include "type.h"

typedef struct {
	// metadata
	usize capacity;
	usize size;
	usize elem_size;

	// elements
	void** elem;
} vec_t;

#define VEC_MAKE(VEC, TYPE) vec_make(&VEC, sizeof(TYPE))
#define VEC_PUSH(VEC, TYPE, ELEM)                                             \
	do {                                                                      \
		TYPE elem = ELEM;                                                     \
		vec_push(&VEC, (void*)&elem);                                         \
	} while (0)
#define VEC_POP(VEC) vec_pop(&VEC)
#define VEC_GET(VEC, TYPE, I) (TYPE*)vec_get(&VEC, I)
#define VEC_BACK(VEC, TYPE) (TYPE*)vec_back(&VEC) 
#define VEC_SIZE(VEC) VEC.size

void vec_make(vec_t* vec, usize elem_size);
void vec_push(vec_t* vec, void* elem);
void vec_pop(vec_t* vec);

static inline void* vec_get(vec_t* vec, usize i) {
	return (i > vec->size) ? NULL : vec->elem[i];
}

static inline void* vec_back(vec_t* vec) {
	return (vec->size == 0) ? NULL : vec->elem[vec->size - 1];
}
