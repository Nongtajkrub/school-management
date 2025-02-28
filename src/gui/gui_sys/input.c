#include "input.h"
#include "renderer.h"

#include <stdlib.h>

void ui_input_make(ui_input_t* in) {
	in->len = 0;
	in->buf = NULL;
}

void ui_input_destroy(ui_input_t* in) {
	if (in->len > 0) {
		free(in->buf);
	}
	in->len = 0;
}

bool ui_input_get(const char* promt, ui_input_t* in) {
	ui_renderer_unready();

	printf("%s -> ", promt);
	fflush(stdout);

	usize alloc_size = 0;
	if (getline(&in->buf, &alloc_size, stdin) == EOF) {
		return false;
	}
	in->len = strlen(in->buf);

	// remove new line char if exsit
	if (in->buf[in->len - 1] == '\n' || in->buf[in->len - 1] == '\r') {
		in->buf[in->len - 1] = '\0';
		in->len--;
	}

	ui_renderer_ready();
	return true;
}
