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

void msg_make(msg_t* msg, char* type, char* fmt, ...);

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

static void msg_add_str(msg_t* msg, const char* value) {
	// + 1 for '/'
	var_string_reserve(msg, strlen(value) + 1);

	var_string_cat_char(msg, '/');
	var_string_cat(msg, value);
}

static void msg_add_i32(msg_t* msg, i32 value) {
	var_string_cat_char(msg, '/');
	var_string_cat_i32(msg, value);
}

static void msg_add_f32(msg_t* msg, f32 value) {
	var_string_cat_char(msg, '/');
	var_string_cat_f32(msg, value);
}

static void msg_add_bool(msg_t* msg, bool value) {
	var_string_cat_char(msg, '/');
	var_string_cat_bool(msg, value);
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

bool msg_send_err(i32 sockfd);

static inline bool msg_is_err(vec_t* parse_msg) {
	return (strcmp(fix_string_get(VEC_GET(parse_msg, fix_string_t, 1)), "Err") == 0);
}

bool msg_recv(msg_t* buf, i32 sockfd);

void msg_parse(vec_t* buf, msg_t* req);

static inline const char* msg_parse_get(vec_t* parse_msg, u32 i) {
	return fix_string_get(VEC_GET(parse_msg, fix_string_t, i));
}

void msg_parse_destroy(vec_t* buf);
