#pragma once

#include "request.h"

#include <type.h>
#include <vector.h>

void req_lex(vec_t* buf, req_t* req);
void req_lex_destroy(vec_t* buf);
