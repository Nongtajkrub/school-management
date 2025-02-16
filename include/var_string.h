#pragma once

#include "type.h"

#ifndef VARSTR_SMALL_BUF_SIZE
	#define VARSTR_SMALL_BUF_SIZE 16
#endif // #ifndef VARSTR_SMALL_BUF_SIZE

typedef struct {
	bool using_small_buf;
	usize capacity;
	usize len;

	char* data;
	char small_buf[VARSTR_SMALL_BUF_SIZE + 1];
} var_string_t;

void var_string_make(var_string_t* str);
void var_string_make_and_set(var_string_t* str, const char* src);

void var_string_set(var_string_t* str, const char* src);
const char* var_string_get(var_string_t* str);

static inline void var_string_output(var_string_t* str) {
	printf("%s\n", var_string_get(str));
}
