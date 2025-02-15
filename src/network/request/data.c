#include "data.h"
#include "err_msg.h"

#include <memory.h>
#include <stdarg.h>

void req_data_destroy(req_data_t* req_data) {
	for (u16 i = 0; i < vec_size(req_data); i++) {
		fix_string_t* data = VEC_GET(req_data, fix_string_t, i);
		fix_string_destroy(data);
	}

	vec_destroy(req_data);
}

void req_data_add_str(req_data_t* req_data, const char* value) {
	fix_string_t str;

	fix_string_make(&str, strlen(value));
	fix_string_set(&str, value);

	vec_push(req_data, &str);
}


void req_data_add_i32(req_data_t* req_data, i32 value) {
	char i32_to_str_buf[MAX_I32_DIGIT + 1];
	memset(i32_to_str_buf, '\0', MAX_I32_DIGIT + 1);
	sprintf(i32_to_str_buf, "%d", value);

	req_data_add_str(req_data, i32_to_str_buf);
}

void req_data_add_f32(req_data_t* req_data, f32 value) {
	char f32_to_str_buf[MAX_F32_DIGIT + 1];
	memset(f32_to_str_buf, '\0', MAX_F32_DIGIT + 1);
	sprintf(f32_to_str_buf, "%f", value);

	req_data_add_str(req_data, f32_to_str_buf);
}

void req_data_add_bool(req_data_t* req_data, bool value) {
	if (value) {
		req_data_add_str(req_data, "true");
	} else {
		req_data_add_str(req_data, "false");
	}
}

void req_data_output(req_data_t* req_data) {
	for (u16 i = 0; i < vec_size(req_data); i++) {
		fix_string_t* data = VEC_GET(req_data, fix_string_t, i);
		printf("%s\n", fix_string_get(data));
	}
}

void req_data_add_fmt(req_data_t* req_data, char* fmt, ...) {
	va_list data;
	const char* c = fmt;

	va_start(data, fmt);

	while (*c != '\0') {
		switch (*c) {
		case 'i':
			req_data_add_i32(req_data, va_arg(data, i32));
			break;
		case 's':
			req_data_add_str(req_data, va_arg(data, char*));
			break;
		case 'f':
			// have to use f64 to prevent promotion
			req_data_add_f32(req_data, va_arg(data, f64));
			break;
		default:
			ASSERT(TRUE, REQ_DATA_ADD_INVALID_FMT_ERRMSG);
			break;
		}

		c++;
	}
}

// return the combine len of all data in req_data
static usize no_format_len(req_data_t* req_data) {
	usize size = 0;

	for (u16 i = 0; i < vec_size(req_data); i++) {
		fix_string_t* data = VEC_GET(req_data, fix_string_t, i);
		size += fix_string_len(data);
	}

	return size;
}

// req_data when format is "/data1/data2/data3"
fix_string_t req_data_format(req_data_t* req_data) {
	// add vec_size for speration charctor (/)
	const usize len = no_format_len(req_data) + vec_size(req_data);
	fix_string_t formated_data;

	fix_string_make(&formated_data, len);

	for (u16 i = 0; i < vec_size(req_data); i++) {
		fix_string_t* data = VEC_GET(req_data, fix_string_t, i);

		fix_string_cat(&formated_data, fix_string_get(data));

		if (i != vec_size(req_data) - 1) {
			fix_string_cat_char(&formated_data, '/');
		}
	}

	return formated_data;
}

/*
req_data_t req_data_parse(char* str) {

}
*/
