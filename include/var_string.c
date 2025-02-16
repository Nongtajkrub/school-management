#include "var_string.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

static void alloc_str(var_string_t* str, usize size) {
	str->capacity = size + 1;
	str->data = malloc(str->capacity);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	memset(str->data, '\0', str->capacity);
}

void var_string_make(var_string_t* str) {
	str->using_small_buf = FALSE;
	str->capacity = 0;
	str->len = 0;
	// allocate only when needed
	str->data = NULL;
	memset(str->small_buf, '\0', VARSTR_SMALL_BUF_SIZE + 1);
}

void var_string_make_and_set(var_string_t* str, const char* src) {
	var_string_make(str);
	var_string_set(str, src);
}

void var_string_set(var_string_t* str, const char* src) {
	const usize src_len = strlen(src);

	// if small enough use small buf instead
	if (src_len < VARSTR_SMALL_BUF_SIZE) {
		str->using_small_buf = TRUE;
		memcpy(str->small_buf, src, src_len);
		return;
	}

	// if to big allocate more space first 
	if (src_len > str->capacity) {
		str->using_small_buf = FALSE;
		alloc_str(str, src_len);
	}

	memcpy(str->data, src, src_len);
}

const char* var_string_get(var_string_t* str) {
	if (str->using_small_buf) {
		return str->small_buf;
	} else {
		return str->data;
	}
}
