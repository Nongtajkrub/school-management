#include "fix_string.h"
#include "err_msg.h"

#include <memory.h>
#include <stdlib.h>

void fix_string_make(fix_string_t* str, usize size) {
	str->size = size;
	str->len = 0;

	str->data = malloc(size + 1);
	ASSERT(str->data != NULL, DEF_ALLOC_ERRMSG);
	memset(str->data, '\0', size + 1);
}

void fix_string_from(fix_string_t* dest, fix_string_t* src) {
	fix_string_make(dest, fix_string_len(src));
	fix_string_set(dest, fix_string_get_raw(src));
}

void fix_string_destroy(fix_string_t* str) {
	str->size = 0;
	free(str->data);
}

fix_string_t fix_string_convert_str(char* src) {
	fix_string_t str;

	fix_string_make(&str, strlen(src));
	fix_string_set(&str, src);

	return str;
}

void fix_string_fill(fix_string_t* str, char c) {
	memset(str->data, c, str->size);
	str->len = str->size;
}

void fix_string_set(fix_string_t* str, const char* src) {
	const usize src_len = strlen(src);
	ASSERT(src_len <= str->size, DEF_OVERFLOW_ERRMSG);

	memcpy(str->data, src, src_len);
	str->len = src_len;
}

void fix_string_set_i(fix_string_t* str, char c, u32 i) {
	ASSERT(i < str->size, DEF_OVERFLOW_ERRMSG);
	ASSERT(i < str->len, FIX_STRING_INDEX_OUT_OF_BOUNDS_ERRMSG);

	str->data[i] = c;
}

void fix_string_cat(fix_string_t* str, const char* src) {
	const usize src_len = strlen(src);
	ASSERT((str->size - str->len) >= src_len, DEF_OVERFLOW_ERRMSG);

	memcpy(str->data + str->len, src, src_len);
	str->len += src_len;
}

void fix_string_cat_char(fix_string_t* str, char c) {
	ASSERT(str->len < str->size, DEF_OVERFLOW_ERRMSG);
	str->data[str->len] = c;
	str->len++;
}
