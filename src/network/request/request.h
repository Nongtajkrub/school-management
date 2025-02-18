// TODO: redo the request system

#pragma once

#include <vector.h>
#include <var_string.h>
#include <stdlib.h>

typedef var_string_t req_t;

#define REQ_TYPE_ID_BY_NAME "ID_BY_NAME"

// fmt - 'i' for int, 's' for string, 'f' for float, 'b' for bool
void req_make(req_t* req, char* type, char* fmt, ...);

static inline void req_destroy(req_t* req) {
	var_string_destroy(req);
}

static inline const char* req_get(req_t* req) {
	return var_string_get(req);
}

bool req_send(req_t* req, i32 sock_fd);
bool req_recv(req_t* req, i32 sock_fd);
