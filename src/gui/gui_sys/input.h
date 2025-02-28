#pragma once

#include <type.h>
#include <stdlib.h>

typedef struct {
	// the lenght of the buffer string
	usize len;

	char* buf;
} ui_input_t;

void ui_input_make(ui_input_t* in);
void ui_input_destroy(ui_input_t* in);

// return whether read is succesful
bool ui_input_get(const char* promt, ui_input_t* in);

static inline char* ui_input_get_buf(ui_input_t* in) {
	return (in->len > 0) ? in->buf : NULL;
}

static inline usize ui_input_get_len(ui_input_t* in) {
	return in->len;
}
