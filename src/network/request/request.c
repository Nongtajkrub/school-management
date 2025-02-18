#include "request.h"
#include "err_msg.h"
#include "../networkio.h"

#include <memory.h>
#include <stdarg.h>

static inline void add_str(req_t* req, char* value) {
	// + 1 for the extra '/' character
	var_string_reserve(req, strlen(value) + 1);
	var_string_cat(req, value);
	var_string_cat_char(req, '/');
}

static void add_i32(req_t* req, i32 value) {
	char i32_to_str_buf[MAX_I32_DIGIT + 1];
	memset(i32_to_str_buf, '\0', MAX_I32_DIGIT + 1);
	sprintf(i32_to_str_buf, "%d", value);

	add_str(req, i32_to_str_buf);
}

static void add_f32(req_t* req, f32 value) {
	char f32_to_str_buf[MAX_F32_DIGIT + 1];
	memset(f32_to_str_buf, '\0', MAX_F32_DIGIT + 1);
	sprintf(f32_to_str_buf, "%f", value);

	add_str(req, f32_to_str_buf);
}

static void add_bool(req_t* req, bool value) {
	if (value) {
		add_str(req, "true");
	} else {
		add_str(req, "false");
	}
}

void req_make(req_t* req, char* type, char* fmt, ...) {
	var_string_make(req);

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
			ASSERT(TRUE, REQ_DATA_ADD_INVALID_FMT_ERRMSG);
			break;
		}

		c++;
	}
}
