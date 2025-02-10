#pragma once

#include <type.h>

#define NAME_BY_ID_ACTION "NAME_BY_ID"
#define NAME_BY_ID_DATA_COUNT 1

typedef struct {
	
} req_data_t;

// take in request type and data
char* make_request(const char* action);
