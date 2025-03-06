#pragma once

#include "container.h"

#include <fix_string.h>
#include <array.h>
#include <ansi_ecs.h>

#define UI_RENDERER_OUT(DELAY, FMT, ...)                                      \
	do {                                                                      \
		ui_renderer_unready();                                                \
		printf(FMT, __VA_ARGS__);                                             \
		fflush(stdout);                                                       \
		sleep(DELAY);                                                         \
		ui_renderer_ready();                                                  \
	} while (0)

typedef struct {
	const char* ansi_esc;
	fix_string_t buf;
} renderer_line_buf_t;

typedef struct {
	u16 w,
		h;

	array_t line_buf;
} ui_renderer_t;

void ui_renderer_ready();
void ui_renderer_unready();

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h);

void ui_renderer_clear(ui_renderer_t* ren);
void ui_render_container(ui_renderer_t* ren, ui_container_t* con);
void ui_renderer_draw(ui_renderer_t* ren);
