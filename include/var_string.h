#pragma once

#include "type.h"

typedef struct {
	usize capacity;
	usize len;

	char* data;
} var_string_t;

void var_string_make(var_string_t* str);
void var_string_make_and_set(var_string_t* str, const char* src);
void var_string_destroy(var_string_t* str);

void var_string_set(var_string_t* str, const char* src);
void var_string_set_i(var_string_t* str, u32 i, char c);

static inline const char* var_string_get(var_string_t* str) {
	return str->data;
}

static inline char* var_string_get_raw(var_string_t* str) {
	return str->data;
}

void var_string_reserve(var_string_t* str, usize size);

void var_string_cat(var_string_t* str, const char* src);
void var_string_cat_char(var_string_t* str, char c);

static inline void var_string_output(var_string_t* str) {
	printf("%s\n", var_string_get(str));
}

static inline usize var_string_len(var_string_t* str) {
	return str->len;
}

// string len including the null terminater
static inline usize var_string_len_null(var_string_t* str) {
	return str->len + 1; 
}
