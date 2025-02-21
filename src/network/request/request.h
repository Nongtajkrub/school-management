#define NETIO_ENABLE_LOG

#pragma once

#include "../networkio.h"

#include <vector.h>
#include <var_string.h>
#include <fix_string.h>
#include <stdlib.h>

/*
 * Reqest format
 *
 * size/type/data1/data2/.../;
 *
 * size - 4 character string ( "0000", "0014", "1024" ) [string]
 * type - request type ( "ID_BY_NAME" ) [string]
 * data* - data needed to process request [string]
 * ; - end of request
*/
typedef var_string_t req_t;

/*
 * ID_BY_NAME Argument list
 * - student name (string)
 *
 * For geting student ID by using their name.
*/
#define REQ_TYPE_ID_BY_NAME "ID_BY_NAME"

// fmt - 'i' for int, 's' for string, 'f' for float, 'b' for bool
void req_make(req_t* req, char* type, char* fmt, ...);

static inline void req_destroy(req_t* req) {
	var_string_destroy(req);
}

static inline const char* req_get(req_t* req) {
	return var_string_get(req);
}

static inline bool req_send(req_t* req, i32 sockfd) {
	return netio_send(sockfd, var_string_get(req), var_string_len(req), TRUE);
}

bool req_recv(req_t* buf, i32 sockfd);
