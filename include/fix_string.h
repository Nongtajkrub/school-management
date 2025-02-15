#pragma once

#include "type.h"

#include <memory.h>

typedef struct {
	char* data;

	usize size;
	usize len;
} fix_string_t;

void fix_string_make(fix_string_t* str, usize size);
void fix_string_destroy(fix_string_t* str);

void fix_string_fill(fix_string_t* str, char c);

static inline const char* fix_string_get(fix_string_t* str) {
	return str->data;
}

void fix_string_set(fix_string_t* str, const char* src);
void fix_string_set_i(fix_string_t* str, char c, u32 i);
void fix_string_cat(fix_string_t* str, const char* src);
void fix_string_cat_char(fix_string_t* str, char c);

static inline usize fix_string_len(fix_string_t* str) {
	return str->len;
}

static inline usize fix_string_size(fix_string_t* str) {
	return str->size;
}

static inline void fix_string_out(fix_string_t* str) {
	printf("%s\n", str->data);
}
