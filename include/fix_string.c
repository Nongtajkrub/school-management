#include "fix_string.h"

#include <memory.h>
#include <stdlib.h>

void fix_string_make(fix_string_t* str, usize size) {
	str->size = size;

	str->data = malloc(size + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	memset(str->data, '\0', size + 1);
}

void fix_string_destroy(fix_string_t* str) {
	str->size = 0;
	free(str->data);
}
