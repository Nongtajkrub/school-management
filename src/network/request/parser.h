#pragma once

#include "request.h"

#include <type.h>
#include <vector.h>

typedef struct {
	data_types_t type;
	fix_string_t data;
} req_data_t;

void req_parse(vec_t* buf, req_t* req);
void req_parse_destroy(vec_t* buf);
