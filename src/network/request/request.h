#pragma once

#include <vector.h>
#include <fix_string.h>
#include <stdlib.h>

typedef vec_t req_t;

#define REQ_TYPE_ID_BY_NAME "ID_BY_NAME"

void req_make(req_t* req, char* type);
void req_destroy(req_t* req);

void req_add_str(req_t* req, const char* value);
void req_add_i32(req_t* req, i32 value);
void req_add_f32(req_t* req, f32 value);
void req_add_bool(req_t* req, bool value);
// 'i' for int, 's' for string, 'f' for float
void req_add_fmt(req_t* req, char* fmt, ...);

void req_output(req_t* req);

bool req_send(req_t* req);
bool req_recv(req_t* req);
