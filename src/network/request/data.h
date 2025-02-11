#pragma once

#include <vector.h>
#include <fix_string.h>
#include <stdlib.h>

typedef vec_t req_data_t;

static inline void req_data_make(req_data_t* req_data) {
	VEC_MAKE(req_data, fix_string_t);
}

void req_data_destroy(req_data_t* req_data);

void req_data_add_str(req_data_t* req_data, const char* value);
void req_data_add_i32(req_data_t* req_data, i32 value);
void req_data_add_f32(req_data_t* req_data, f32 value);
void req_data_add_bool(req_data_t* req_data, bool value);

void req_data_output(req_data_t* req_data);

// req_data when format is "/data1/data2/data3"
char* req_data_format(req_data_t* req_data);

static inline void req_data_format_destroy(char* formated_data) {
	free(formated_data);
}
