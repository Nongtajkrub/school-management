#pragma once

#include <type.h>

typedef enum : u8 {
	GET_ID
} req_type_t;

void cli_main();

void cli_req(req_type_t type);
