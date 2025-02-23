#include "handler.h"

#include <ctype.h>

#define REQ_DATA_BEGIN_INDEX 2
#define REQ_TYPE_INDEX 1

typedef enum: u8 {
	UNKOWN,
	ID_BY_NAME
} req_type_t;

static req_type_t resolve_req_type(vec_t* parsed_req) {
	fix_string_t* type = VEC_GET(parsed_req, fix_string_t, REQ_TYPE_INDEX);
	const char* str = fix_string_get(type);

	if (strcmp(str, REQ_TYPE_ID_BY_NAME) == 0) return ID_BY_NAME;

	return UNKOWN;
}

static const char* get_data_fmt_by_type(req_type_t type) {
	switch(type) {
	case ID_BY_NAME: 
		return REQ_TYPE_ID_BY_NAME_DATA_FMT;
	default:
		return NULL;
	}
}

// get the parse request size with out the meta data (no size and type)
static inline usize get_parsed_req_size_no_meta(vec_t* parsed_req) {
	return vec_size(parsed_req) - 2;
}

static bool is_token_i32(fix_string_t* token) {
	const char* str = fix_string_get(token);

	// handle empty and lone '-'
	if (*str == '\0' || (*str == '-' && !isdigit(str[1]))) { 
		return 0;
	}

	char* endptr;
	strtol(str, &endptr, 10);

	// valid if nothing remains after number
	return *endptr == '\0';
}

static bool is_token_f32(fix_string_t* token) {
	const char* str = fix_string_get(token);

    if (*str == '\0') {
		return 0;
	}

    char *endptr;
    strtod(str, &endptr);

	// valid if nothing remains after number
    return *endptr == '\0'; 
}

static bool is_token_bool(fix_string_t* token) {
	const char* str = fix_string_get(token);

	if ((strcmp(str, "true") == 0) || (strcmp(str, "false") == 0)) { 
		return true;
	}

	return false;
} 

static inline bool is_token_str(fix_string_t* token) {
	return !(
		is_token_i32(token) || is_token_bool(token) || is_token_f32(token));
}

static bool is_token_valid_type(fix_string_t* token, char fmt) {
	switch(fmt) {
	case 's':
		if (!is_token_str(token)) {
			return false;
		}
		break;
	case 'i':
		if (!is_token_i32(token)) { 
			return false;
		}
		break;
	case 'f':
		if (!is_token_f32(token)) {
			return false;
		}
		break;
	case 'b':
		if (!is_token_bool(token)) {
			return false;
		}
		break;
	default:
		return false;
	}

	return true;
}

bool is_valid_req(vec_t* parsed_req) {
	const req_type_t type = resolve_req_type(parsed_req);
	
	if (type == UNKOWN) {
		return false;
	}

	const usize size = vec_size(parsed_req);
	const char* data_fmt = get_data_fmt_by_type(type);

	// more data in req than suppose to have 
	if (get_parsed_req_size_no_meta(parsed_req) > strlen(data_fmt)) {
		return false;
	}

	// loop through all data and check whether the type is 
	// correct using the data_fmt
	for (usize i = REQ_DATA_BEGIN_INDEX; i < size; i++) {
		fix_string_t* token = VEC_GET(parsed_req, fix_string_t, i);

		if (!is_token_valid_type(token, *data_fmt)) {
			return false;
		}

		data_fmt++;
	}

	return true;
}

bool req_handle(req_t* req, i32 sockfd, dbdata_t* db) {
	vec_t parsed_req;
	req_lex(&parsed_req, req);

	if (!is_valid_req(&parsed_req)) {
		return false;
	}

	return true;
}
