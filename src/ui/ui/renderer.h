#pragma once

#include "ui.h"

#include <array.h>

typedef struct {
	// width and height
	u16 w,
		h;

	// rendering buffer
	usize buf_size;
	array_t buf;
} ui_renderer_t;

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h);

void ui_render_component(ui_renderer_t* ren, ui_component_t* comp);
void ui_render_menu(ui_renderer_t* ren, ui_menu_t* menu);

void ui_renderer_clear(ui_renderer_t* ren);
void ui_renderer_draw(ui_renderer_t* ren);
