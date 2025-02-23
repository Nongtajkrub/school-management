#include "lexer.h"

#include <fix_string.h>
#include <string.h>

#define SEPERATOR_CHAR '/'
#define END_REQ_CHAR ';'

#define REQ_TYPE_INDEX 1

static void handle_seperator(vec_t* buf, var_string_t* lexeme) {
	fix_string_t lexeme_buf_entry;
	vec_push(buf, &lexeme_buf_entry);

	fix_string_t* new_lexeme = VEC_GET(buf, fix_string_t, vec_size(buf) - 1);

	fix_string_make(new_lexeme, var_string_len(lexeme));
	fix_string_set(new_lexeme, var_string_get(lexeme));

	var_string_clear(lexeme);
}

void req_lex(vec_t* buf, req_t* req) {
	VEC_MAKE(buf, fix_string_t);

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
}
