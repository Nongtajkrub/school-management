#pragma once

#include <stdlib.h>

// if no input occure return NULL
char* ui_input_get(const char* promt);

static inline void ui_input_delete(char* buf) {
	free(buf);
}
