#pragma once

#include <type.h>
#include <vector.h>

void cli_main();

bool cli_req_block_by_name(const char* name, vec_t* buf);
