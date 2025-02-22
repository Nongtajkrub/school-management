#pragma once

#include "request.h"

#include <type.h>
#include <vector.h>

void req_parse(vec_t* buf, req_t* req);
void req_parse_destroy(vec_t* buf);
