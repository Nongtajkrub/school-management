#include "parser.h"

#include <fix_string.h>

#define SEPERATOR_CHAR '/'
#define END_REQ_CHAR ';'

#define REQ_TYPE_INDEX 1

static void handle_seperator(vec_t* buf, var_string_t* lexeme) {
	req_data_t new_req_data = { .type = TYPE_UNKNOWN };

	vec_push(buf, &new_req_data);
	req_data_t* req_data = vec_back(buf);

	fix_string_make(&req_data->data, var_string_len(lexeme));
	fix_string_set(&req_data->data, var_string_get(lexeme));

	var_string_clear(lexeme);
}

static void resolve_types(vec_t* buf) {
	req_data_t* req_type = VEC_GET(buf, req_data_t, REQ_TYPE_INDEX); 
}

void req_parse(vec_t* buf, req_t* req) {
	VEC_MAKE(buf, req_data_t);

	const char* c = var_string_get(req);

	var_string_t lexeme;
	var_string_make(&lexeme);
	var_string_reserve(&lexeme, 30);

	while (*c != '\0') {
		switch(*c) {
		case SEPERATOR_CHAR:
		case END_REQ_CHAR:
			handle_seperator(buf, &lexeme);
			break;
		default:
			var_string_cat_char(&lexeme, *c);
			break;
		}

		c++;
	}

	var_string_destroy(&lexeme);
}

void req_parse_destroy(vec_t* buf) {
	for (u32 i = 0; i < vec_size(buf); i++) {
		req_data_t* data = VEC_GET(buf, req_data_t, i);
		fix_string_destroy(&data->data);
	}

	vec_destroy(buf);
}
