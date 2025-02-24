#include "request.h"
#include "err_msg.h"

#include <memory.h>
#include <stdarg.h>
#include <math.h>
#include <fix_string.h>

static void add_str(msg_req_t* req, const char* value) {
	// + 1 for '/'
	msg_reserve(req, strlen(value) + 1);

	msg_cat_char(req, '/');
	msg_cat(req, value);
}

static void add_i32(msg_req_t* req, i32 value) {
	msg_cat_char(req, '/');
	msg_cat_i32(req, value);
}

static void add_f32(msg_req_t* req, f32 value) {
	msg_cat_char(req, '/');
	msg_cat_f32(req, value);
}

static void add_bool(msg_req_t* req, bool value) {
	msg_cat_char(req, '/');
	msg_cat_bool(req, value);
}

void req_make(msg_req_t* req, char* type, char* fmt, ...) {
	msg_begin(req);

	add_str(req, type);

	va_list data;
	const char* c = fmt;

	va_start(data, fmt);

	while (*c != '\0') {
		switch (*c) {
		case 'i':
			add_i32(req, va_arg(data, i32));
			break;
		case 's':
			add_str(req, va_arg(data, char*));
			break;
		case 'f':
			// have to use f64 to prevent promotion
			add_f32(req, va_arg(data, f64));
			break;
		case 'b':
			// have to use i32 to prevent promotion
			add_bool(req, va_arg(data, i32));
		default:
			ASSERT(true, REQ_DATA_ADD_INVALID_FMT_ERRMSG);
			break;
		}

		c++;
	}

	va_end(data);

	msg_end(req);
}
