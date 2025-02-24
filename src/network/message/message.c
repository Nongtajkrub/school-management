#include "message.h"

#include <memory.h>
#include <stdarg.h>
#include <math.h>
#include <fix_string.h>

#define SIZE_STR_LEN 4

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

static void encode_size(msg_t* msg, usize size) {
	const u8 digit_count = get_digit_count(size);
	const u8 start_i = (SIZE_STR_LEN - digit_count);
	const u8 end_i = start_i + digit_count;
	u8 current_digit = digit_count;

	ASSERT(digit_count <= SIZE_STR_LEN, DEF_OVERFLOW_ERRMSG);

	for (u8 i = start_i; i < end_i; i++) {
		var_string_set_i(msg, i, INT_TO_CAHR(get_digit(size, current_digit)));
		current_digit--;
	}
}

void msg_begin(msg_t* msg) {
	var_string_make(msg);
	var_string_cat(msg, "0000");
}

void msg_end(msg_t* msg) {
	var_string_cat_char(msg, ';');
	encode_size(msg, var_string_len(msg));
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

static usize recv_msg_size(var_string_t* buf, i32 sockfd) {
	char size_str[SIZE_STR_LEN + 1];
	memset(size_str, '\0', SIZE_STR_LEN + 1);

	if (!netio_recv(sockfd, size_str, SIZE_STR_LEN, true)) {
		return 0;
	}

	const usize size = decode_size(size_str);

	var_string_reserve(buf, size);
	var_string_cat(buf, size_str);

	return size;
}

bool msg_recv(msg_t* buf, i32 sockfd) {
	var_string_make(buf);

	// recv the size and add it to the buffer
	const usize size = recv_msg_size(buf, sockfd);

	if (size == 0) {
		var_string_destroy(buf);
		return false;
	}

	// recv the rest of the msguest
	if (!netio_recv(
			sockfd,
		   	var_string_get_raw(buf) + SIZE_STR_LEN, 
			size - SIZE_STR_LEN, true)) {
		var_string_destroy(buf);
		return false;
	}

	return true;
}
