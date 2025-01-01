#pragma once

// forward declear for ui.h
typedef struct ui_component ui_component_t;
typedef struct ui_menu ui_menu_t;

#include <vector.h>
#include <fix_string.h>

typedef struct ui_renderer {
	// width and height
	u16 w,
		h;

	// rendering buffer
	fix_string_t buf;
} ui_renderer_t;

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h);

void ui_render_component(ui_renderer_t* ren, ui_component_t* comp);
void ui_render_menu(ui_renderer_t* ren, ui_menu_t* menu);

void ui_renderer_clear(ui_renderer_t* ren);

static inline void ui_renderer_draw(ui_renderer_t* ren) {
	// clear the screen move the cursor to the top and print
	// the render buffer
	printf("\033[2J\033[H%s\n", fix_string_get(&ren->buf));
}
