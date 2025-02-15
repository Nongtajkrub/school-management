#include "request.h"
#include "err_msg.h"

#include <memory.h>
#include <stdarg.h>

void req_make(req_t* req, char* type) {
	VEC_MAKE(req, fix_string_t);

	// convert from c string to fix_string_t and push to req
	fix_string_t type_fix_string = fix_string_convert_str(type);
	vec_push(req, &type_fix_string);
}

void req_destroy(req_t* req) {
	for (u16 i = 0; i < vec_size(req); i++) {
		fix_string_t* data = VEC_GET(req, fix_string_t, i);
		fix_string_destroy(data);
	}

	vec_destroy(req);
}

void req_add_str(req_t* req, const char* value) {
	fix_string_t str;

	fix_string_make(&str, strlen(value));
	fix_string_set(&str, value);

	vec_push(req, &str);
}


void req_add_i32(req_t* req, i32 value) {
	char i32_to_str_buf[MAX_I32_DIGIT + 1];
	memset(i32_to_str_buf, '\0', MAX_I32_DIGIT + 1);
	sprintf(i32_to_str_buf, "%d", value);

	req_add_str(req, i32_to_str_buf);
}

void req_add_f32(req_t* req, f32 value) {
	char f32_to_str_buf[MAX_F32_DIGIT + 1];
	memset(f32_to_str_buf, '\0', MAX_F32_DIGIT + 1);
	sprintf(f32_to_str_buf, "%f", value);

	req_add_str(req, f32_to_str_buf);
}

void req_add_bool(req_t* req, bool value) {
	if (value) {
		req_add_str(req, "true");
	} else {
		req_add_str(req, "false");
	}
}

void req_output(req_t* req) {
	for (u16 i = 0; i < vec_size(req); i++) {
		fix_string_t* data = VEC_GET(req, fix_string_t, i);
		printf("%s\n", fix_string_get(data));
	}
}

void req_add_fmt(req_t* req, char* fmt, ...) {
	va_list data;
	const char* c = fmt;

	va_start(data, fmt);

	while (*c != '\0') {
		switch (*c) {
		case 'i':
			req_add_i32(req, va_arg(data, i32));
			break;
		case 's':
			req_add_str(req, va_arg(data, char*));
			break;
		case 'f':
			// have to use f64 to prevent promotion
			req_add_f32(req, va_arg(data, f64));
			break;
		default:
			ASSERT(TRUE, REQ_DATA_ADD_INVALID_FMT_ERRMSG);
			break;
		}

		c++;
	}
}
