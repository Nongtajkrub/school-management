#include "var_string.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

static void alloc_str(var_string_t* str, usize size) {
	if (str->data != NULL) {
		free(str->data);
	}

	str->capacity = size;
	str->len = 0;
	str->data = malloc(str->capacity + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	memset(str->data, '\0', str->capacity + 1);
}

static void realloc_str(var_string_t* str, usize size) {
	str->capacity = size;
	str->data = realloc(str->data, str->capacity + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	// clear everything except the old data
	memset(str->data + str->len, '\0', (str->capacity - str->len) + 1);
}

static inline void clear_small_buf(var_string_t* str) {
	memset(str->small_buf, '\0', VARSTR_SMALL_BUF_SIZE + 1);
}

void var_string_make(var_string_t* str) {
	str->using_small_buf = FALSE;
	str->capacity = 0;
	str->len = 0;
	// allocate only when needed
	str->data = NULL;
}

void var_string_make_and_set(var_string_t* str, const char* src) {
	var_string_make(str);
	var_string_set(str, src);
}

void var_string_destroy(var_string_t* str) {
	str->using_small_buf = FALSE;
	str->capacity = 0;
	str->len = 0;

	if (str->data != NULL) {
		free(str->data);
	}

	clear_small_buf(str);
}

void var_string_set(var_string_t* str, const char* src) {
	const usize src_len = strlen(src);

	// if small enough use small buf instead
	if (src_len < VARSTR_SMALL_BUF_SIZE) {
		str->using_small_buf = TRUE;
		clear_small_buf(str);
		memcpy(str->small_buf, src, src_len);
		return;
	}

	// if to big allocate more space first 
	if (src_len > str->capacity) {
		str->using_small_buf = FALSE;
		alloc_str(str, src_len);
	}

	memcpy(str->data, src, src_len);
	str->len = src_len;
}

const char* var_string_get(var_string_t* str) {
	if (str->using_small_buf) {
		return str->small_buf;
	} else {
		return str->data;
	}
}

void var_string_cat(var_string_t* str, const char* src) {
	const usize src_len = strlen(src);
	const usize space_left = str->capacity - str->len;

	// if not enough space and str is not new realloc the string else alloc
	if (space_left < src_len) {
		str->capacity += src_len - space_left;

		// if the string is not empty realloc it else alloc
		if (str->data != NULL) {
			realloc_str(str, str->capacity);
		} else {
			alloc_str(str, str->capacity);
		}
	}

	memcpy(str->data + str->len, src, src_len);
	str->len += src_len;
}
