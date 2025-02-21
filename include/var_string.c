#include "var_string.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

static void str_malloc(var_string_t* str, usize size) {
	if (str->data != NULL) {
		free(str->data);
	}

	str->capacity = size;
	str->len = 0;
	str->data = malloc(str->capacity + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	memset(str->data, '\0', str->capacity + 1);
}

static void str_realloc(var_string_t* str, usize size) {
	str->capacity = size;
	str->data = realloc(str->data, str->capacity + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	// clear everything except the old data
	memset(str->data + str->len, '\0', (str->capacity - str->len) + 1);
}

// if the string is not empty realloc else malloc
static inline void use_correct_method_to_alloc(var_string_t* str, usize size) {
	if (str->data != NULL) {
		str_realloc(str, size);
	} else {
		str_malloc(str, size);
	}
}

void var_string_make(var_string_t* str) {
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
	str->capacity = 0;
	str->len = 0;

	if (str->data != NULL) {
		free(str->data);
	}
}

void var_string_set(var_string_t* str, const char* src) {
	const usize src_len = strlen(src);

	// if src to big allocate more space first 
	if (src_len > str->capacity) {
		str_malloc(str, src_len);
	}

	memcpy(str->data, src, src_len);
	str->len = src_len;
}

void var_string_set_i(var_string_t* str, u32 i, char c) {
	ASSERT(str->len > i, DEF_OVERFLOW_ERRMSG);
	str->data[i] = c;
}

void var_string_clear(var_string_t* str) {
	memset(str->data, '\0', str->len);
	str->len = 0;
}

void var_string_reserve(var_string_t* str, usize size) {
	str->capacity += size;
	use_correct_method_to_alloc(str, str->capacity);
}

// ensure capacity is enought to cat string
static void ensure_capacity_to_cat(var_string_t* str, usize src_len) {
	const usize space_left = str->capacity - str->len;

	// if not enough space alloc more
	if (space_left < src_len) {
		str->capacity += src_len - space_left;

		use_correct_method_to_alloc(str, str->capacity);
	}
}

void var_string_cat(var_string_t* str, const char* src) {
	const usize src_len = strlen(src);
	ensure_capacity_to_cat(str, src_len);

	memcpy(str->data + str->len, src, src_len);
	str->len += src_len;
}

void var_string_cat_char(var_string_t* str, char c) {
	ensure_capacity_to_cat(str, 1);

	str->data[str->len] = c;
	str->len++;
}
