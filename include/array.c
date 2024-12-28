#include "array.h"

void array_make(array_t* arr, usize size, usize elem_size) {
	arr->size = size;
	arr->elem_size = elem_size;
	
	arr->elem = calloc(size, sizeof(void*)); 
	ASSERT(arr->elem != NULL, DEF_ALLOC_ERRMSG);

	// prealloact all element
	for (usize i = 0; i < size; i++) {
		arr->elem[i] = malloc(elem_size);
		ASSERT(arr->elem[i] != NULL, DEF_ALLOC_ERRMSG);
	}
}
