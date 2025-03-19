#pragma once

#include <type.h>
#include <vector.h>

void cli_main();

// buffer is a vector that contain fix_string_t
bool cli_req_id_by_name(const char* name, vec_t* buf);
void cli_req_id_by_name_destroy(vec_t* buf);
