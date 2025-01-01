#include "renderer.h"
#include "menu.h"
#include "component.h"

#include <memory.h>
#include <stdlib.h>

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h) {
	ren->w = w;
	ren->h = h;

	// make screen buffer
	fix_string_make(&ren->buf, (w * h));
	ui_renderer_clear(ren);
}

void ui_renderer_clear(ui_renderer_t* ren) {
	fix_string_fill(&ren->buf, ' ');

	// add new line characters
	for (u32 h = 0; h < ren->h; h++) {
		if (h == 0) {
			fix_string_set_i(&ren->buf, '\n', ren->w - 1);
			continue;
		}

		// set the char at the last index of each row to a new line
		// if at the last row set the last char to whitespace
		fix_string_set_i(
			&ren->buf,
			(h == ren->h - 1) ? ' ' : '\n',
			(ren->w * (h + 1)) - 1
			);
	}
}

static inline u32 calc_i_from_pos(u16 x, u16 y, u16 w) {
	return (y * w) + x;
}

void ui_render_component(ui_renderer_t* ren, ui_component_t* comp) {
	const u32 start_pos = 
		calc_i_from_pos(
			comp->pos.x,
			comp->pos.y,
			ren->w
		);
	const usize end_pos = start_pos + strlen(comp->label);
	u32 char_index_counter = 0;

	for (u32 i = start_pos; i < end_pos; i++) {
		fix_string_set_i(&ren->buf, comp->label[char_index_counter], i);
		char_index_counter++;
	}
}

void ui_render_menu(ui_renderer_t* ren, ui_menu_t* menu) {
	for (u32 i = 0; i < vec_size(&menu->components); i++) {
		ui_component_t* comp = VEC_GET(&menu->components, ui_component_t, i);
		ui_render_component(ren, comp);
	}
}
