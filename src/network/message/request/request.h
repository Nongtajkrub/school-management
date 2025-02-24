#define NETIO_ENABLE_LOG

#pragma once

#include "../../networkio.h"
#include "../message.h"

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
typedef msg_t msg_req_t;

// fmt - 'i' for int, 's' for string, 'f' for float, 'b' for bool
void req_make(msg_req_t* req, char* type, char* fmt, ...);
