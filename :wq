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
	return !(is_token_i32(token) || is_token_bool(token) || is_token_f32(token));
}

static bool is_token_valid_type(fix_string_t* token, char fmt) {
	switch(fmt) {
	case 's':
		return is_token_str(token);
	case 'i':
		return is_token_i32(token);  
	case 'f':
		return is_token_f32(token);
	case 'b':
		return is_token_bool(token);
	default:
		return false;
	}

	return true;
}

bool is_valid_req(vec_t* parsed_req, req_type_t type) {
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
	//printf("name -> %s\n", name);

	if (str_contain_num(name)) {
		return false;
	}

	vec_t blocks;

	if (!database_find_block_by_name(db, name, &blocks) || vec_size(&blocks) == 0) {
		msg_send_err(sockfd);
		vec_destroy(&blocks);
		return false;
	}

	msg_t reply;

	msg_begin(&reply);

	for (u32 i = 0; i < vec_size(&blocks); i++) {
		database_block_info_t* info = VEC_GET(&blocks, database_block_info_t, i);
		msg_add_i32(&reply, info->block.id);
	}

	msg_end(&reply);

	if (!msg_send(&reply, sockfd)) {
		msg_destroy(&reply);
		return false;
	}

	msg_destroy(&reply);
	return true;
}

bool req_handle(msg_req_t* req, i32 sockfd, database_t* db) {
	bool ret_value = true;

	vec_t parsed_req;
	msg_parse(&parsed_req, req);

	const req_type_t type = resolve_req_type(&parsed_req);

	if (!is_valid_req(&parsed_req, type)) {
		msg_parse_destroy(&parsed_req);
		return false;
	}

	switch(type) {
	case ID_BY_NAME:
		ret_value = handle_req_id_by_name(&parsed_req, sockfd, db);
		break;
	default:
		ret_value = false;
		break;
	}

	msg_parse_destroy(&parsed_req);
	return ret_value;
}
