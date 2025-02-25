#include "handler.h"
#include "req_type.h"

#include <ctype.h>

#define REQ_DATA_BEGIN_INDEX 2
#define REQT_INDEX 1

#define ID_BY_NAME_NAME_INDEX 2

typedef enum: u8 {
	UNKOWN,
	ID_BY_NAME
} req_type_t;

static req_type_t resolve_req_type(vec_t* parsed_req) {
	fix_string_t* type = VEC_GET(parsed_req, fix_string_t, REQT_INDEX);
	const char* str = fix_string_get(type);

	if (strcmp(str, REQT_ID_BY_NAME) == 0) return ID_BY_NAME;

	return UNKOWN;
}

static const char* get_data_fmt_by_type(req_type_t type) {
	switch(type) {
	case ID_BY_NAME: 
		return REQT_ID_BY_NAME_DATA_FMT;
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

bool is_valid_req(vec_t* parsed_req, req_type_t type) {
	if (type == UNKOWN) {
		printf("Unknow request type\n");
		return false;
	}

	const usize size = vec_size(parsed_req);
	const char* data_fmt = get_data_fmt_by_type(type);

	// more data in req than suppose to have 
	if (get_parsed_req_size_no_meta(parsed_req) > strlen(data_fmt)) {
		printf("Invalid request data size\n");
		return false;
	}

	// loop through all data and check whether the type is 
	// correct using the data_fmt
	for (usize i = REQ_DATA_BEGIN_INDEX; i < size; i++) {
		fix_string_t* token = VEC_GET(parsed_req, fix_string_t, i);

		if (!is_token_valid_type(token, *data_fmt)) {
			printf("Invalid request\n");
			return false;
		}

		data_fmt++;
	}

	return true;
}

static bool str_contain_num(const char* str) {
	const char* c = str;

	while (*c != '\0') {
		if (isdigit(*c)) {
			return true;
		}

		c++;
	}

	return false;
}

bool handle_req_id_by_name(vec_t* parsed_req, i32 sockfd, database_t* db) {
	const char* name = 
		fix_string_get(VEC_GET(parsed_req, fix_string_t, ID_BY_NAME_NAME_INDEX));

	if (str_contain_num(name)) {
		printf("Name contain number\n");
		return false;
	}

	i32 id = database_id_by_name(db, name);

	if (id < 0) {
		printf("Id not found\n");
		return false;
	}

	msg_t reply;

	msg_begin(&reply);
	msg_cat_i32(&reply, id);
	msg_end(&reply);

	return msg_send(&reply, sockfd);
}

bool req_handle(msg_req_t* req, i32 sockfd, database_t* db) {
	vec_t parsed_req;
	req_lex(&parsed_req, req);

	const req_type_t type = resolve_req_type(&parsed_req);

	if (!is_valid_req(&parsed_req, type)) {
		return false;
	}

	switch(type) {
	case ID_BY_NAME:
		return handle_req_id_by_name(&parsed_req, sockfd, db);
	default:
		return false;
	}

	return true;
}
