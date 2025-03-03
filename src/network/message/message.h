#define NETIO_ENABLE_LOG

#pragma once

#include "../networkio.h"

#include <vector.h>
#include <var_string.h>
#include <fix_string.h>
#include <stdlib.h>

#define SIZE_STR_LEN 4

typedef var_string_t msg_t;

void msg_begin(msg_t* msg);
void msg_end(msg_t* msg);

static inline void msg_destroy(msg_t* msg) {
	var_string_destroy(msg);
}

static inline const char* msg_get(msg_t* msg) {
	return var_string_get(msg);
}

// get the data portion only "0014Hello;" get "Hello"
static inline char* msg_get_data(msg_t* msg) {
	return var_string_get_snippet(msg, SIZE_STR_LEN, var_string_len(msg) - 1);
}

static inline void msg_get_data_destroy(char* data) {
	free(data);
}

static inline void msg_cat(msg_t* msg, const char* src) {
	var_string_cat(msg, src);
}

static inline void msg_cat_char(msg_t* msg, char c) {
	var_string_cat_char(msg, c);
}

static inline void msg_cat_i32(msg_t* msg, i32 n) {
	var_string_cat_i32(msg, n);
}

static inline void msg_cat_f32(msg_t* msg, f32 f) {
	var_string_cat_f32(msg, f);
}

static inline void msg_cat_bool(msg_t* msg, bool b) {
	var_string_cat_bool(msg, b);
}

static inline void msg_reserve(msg_t* msg, usize size) {
	var_string_reserve(msg, size);
}

static inline usize msg_len(msg_t* msg) {
	return var_string_len(msg);
}

static inline bool msg_send(msg_t* msg, i32 sockfd) {
	return netio_send(sockfd, var_string_get(msg), var_string_len(msg), true);
}

bool msg_recv(msg_t* buf, i32 sockfd);
