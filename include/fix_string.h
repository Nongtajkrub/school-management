#pragma once

#include "type.h"

#include <memory.h>

// fix size string
typedef struct {
	usize size;
	char* data;
} fix_string_t;

void fix_string_make(fix_string_t* str, usize size);
void fix_string_destroy(fix_string_t* str);

static inline void fix_string_fill(fix_string_t* str, char c) {
	memset(str->data, c, str->size);
}

static inline char* fix_string_get(fix_string_t* str) {
	return str->data;
}

static inline void fix_string_set(fix_string_t* str, const char* src) {
	memcpy(str->data, src, strlen(src));
}

static inline void fix_string_set_i(fix_string_t* str, char c, u32 i) {
	str->data[i] = c;
}

static inline usize fix_string_len(fix_string_t* str) {
	return strlen(str->data);
}

static inline usize fix_string_size(fix_string_t* str) {
	return str->size;
}
