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
void var_string_clear(var_string_t* str);

static inline const char* var_string_get(var_string_t* str) {
	return str->data;
}

static inline char* var_string_get_raw(var_string_t* str) {
	return str->data;
}

char* var_string_get_snippet(var_string_t* str, u32 b, u32 n);

static inline void var_string_snippet_destroy(char* snippet) {
	free(snippet);
}

void var_string_reserve(var_string_t* str, usize size);

void var_string_cat(var_string_t* str, const char* src);
void var_string_cat_char(var_string_t* str, char c);
void var_string_cat_i32(var_string_t* str, i32 n);
void var_string_cat_f32(var_string_t* str, f32 f);

static inline void var_string_cat_bool(var_string_t* str, bool b) {
	var_string_cat(str, (b) ? "true" : "false");
}

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
