#include "request.h"
#include "err_msg.h"

#include <memory.h>
#include <stdarg.h>
#include <math.h>

#define SIZE_STR_LEN 4

static void init_header(req_t* req, char* type) {
	// + 5 for '/' and size character
	var_string_reserve(req, strlen(type) + 5);

	var_string_cat(req, "0000");
	var_string_cat_char(req, '/');
	var_string_cat(req, type);
}

static inline void add_str(req_t* req, char* value) {
	// + 1 for the extra '/' character
	var_string_reserve(req, strlen(value) + 1);
	var_string_cat_char(req, '/');
	var_string_cat(req, value);
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

// very fast (Not my code)
static u8 get_digit_count(u32 n) {
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;

    return 10;
}

// get digit from right
// eaxmple: 1234 digit 2 is 3
static u8 get_digit(u32 n, u8 digit) {
	ASSERT(digit <= get_digit_count(n), NO_ERRMSG);
	return (n / (int)pow(10, digit - 1)) % 10;;
}

static void encode_size(req_t* req) {
	const usize size = var_string_len(req);
	const u8 digit_count = get_digit_count(size);
	const u8 start_i = (SIZE_STR_LEN - digit_count);
	const u8 end_i = start_i + digit_count;
	u8 current_digit = digit_count;

	ASSERT(digit_count <= SIZE_STR_LEN, DEF_OVERFLOW_ERRMSG);

	for (u8 i = start_i; i < end_i; i++) {
		var_string_set_i(req, i, INT_TO_CAHR(get_digit(size, current_digit)));
		current_digit--;
	}
}

void req_make(req_t* req, char* type, char* fmt, ...) {
	var_string_make(req);
	init_header(req, type);

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

	va_end(data);

	// end character
	var_string_cat_char(req, ';');
	encode_size(req);
}

// example: "0014" first digit offset if 2, "0104" first digit offset is 1
static usize get_first_digit_offset(char* size_str) {
	const char* c = size_str;
	u8 offset = 0;

	while (*c != '\0' && *c == '0') {
		c++;
		offset++;
	}

	return offset;
}

// example: convert "0014" to 14
static usize decode_size(char* size_str) {
	const u8 first_digit_offset = get_first_digit_offset(size_str);

	const char* c = size_str;
	c += first_digit_offset;

	// digit end at 0, similar to index
	u8 digit = (SIZE_STR_LEN - first_digit_offset) - 1;
	usize result = 0;

	while (*c != '\0') {
		// result += int(c) * (10^digit)
		result += (u16)(CHAR_TO_INT(*c) * pow(10, digit));

		c++;
		digit--;
	}

	return result;
}

static usize recv_req_size(i32 sockfd) {
	char size_str[SIZE_STR_LEN + 1];
	memset(size_str, '\0', SIZE_STR_LEN + 1);

	if (!netio_recv(sockfd, size_str, SIZE_STR_LEN, TRUE)) {
		return 0;
	}

	return decode_size(size_str);
}

bool req_recv(req_t* req, i32 sockfd) {
	ASSERT(TRUE, DEF_NOT_IMPLEMENTED_ERRMSG);
	return FALSE;
}
