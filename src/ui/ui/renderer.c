// TODO: correctly render menu

#include "renderer.h"

#include <memory.h>
#include <stdlib.h>

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h) {
	ren->w = w;
	ren->h = h;
	
	// make screen buffer
	fix_string_make(&ren->buf, w * h);
	ui_renderer_clear(ren);
}

static inline u32 calc_i_from_pos(u16 x, u16 y, u16 w) {
	return y * w + x;
}

void ui_render_component(ui_renderer_t* ren, ui_component_t* comp) {
	const u32 start_pos = 
		calc_i_from_pos(
			comp->pos.y,
			comp->pos.x,
			ren->w
		); 
	u32 char_index_counter = 0;

	for (u32 i = start_pos; i < strlen(comp->label); i++) {
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
