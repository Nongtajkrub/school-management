#include "renderer.h"

#include <array.h>

void ui_make_renderer(ui_renderer_t* ren, u16 w, u16 h) {
	ren->dimension.w = w;
	ren->dimension.h = h;
	
	// make screen buffer
	usize buf_size = w * h;
	
	ARRAY_MAKE(&ren->buf, char, buf_size);
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
			ren->dimension.w
		); 
	u32 char_index_counter = 0;

	for (u32 i = start_pos; i < strlen(comp->label); i++) {
		array_set(&ren->buf, i, (void*)(&comp->label[char_index_counter]));
		char_index_counter++;
	}
}

void ui_render_menu(ui_renderer_t* ren, ui_menu_t* menu) {
	for (u32 i = 0; vec_size(&menu->components); i++) {
		ui_component_t* comp = VEC_GET(&menu->components, ui_component_t, i);
		ui_render_component(ren, comp);
	}
}

void ui_renderer_clear(ui_renderer_t* ren) {
	for (u32 i = 0; i < array_size(&ren->buf); i++) {
		ARRAY_SET(&ren->buf, char, ' ', i);
	}
}

// TODO: test renderer
void ui_renderer_draw(ui_renderer_t* ren) {
	printf("%s\n", (char*)ren->buf.elem);
}
