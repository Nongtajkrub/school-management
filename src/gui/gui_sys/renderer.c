#include "renderer.h"

#include <math.h>
#include <stdlib.h>

void ui_renderer_ready() {
	system("clear");
	printf(ANSI_ESC_CURSOR_HIDE);
}

void ui_renderer_unready() {
#ifdef _WIN32
	system("cls");
#elif __linux__
	system("clear");
#endif

	printf("%s%s", ANSI_ESC_CURSOR_HOME, ANSI_ESC_CURSOR_SHOW);
}

static inline renderer_line_buf_t* get_line_buf(array_t* line_buf, u16 line) {
	return ARRAY_GET(line_buf, renderer_line_buf_t, line);
}

static void make_line_buf(ui_renderer_t* ren) {
	ARRAY_MAKE(&ren->line_buf, renderer_line_buf_t, ren->h);

	for (u16 i = 0; i < array_size(&ren->line_buf); i++) {
		// the following process is to ensure memory safety
		// add an uninit line buf to array
		renderer_line_buf_t def_line_buf;
		array_set(&ren->line_buf, i, &def_line_buf);

		// retrive the uninit line buf from the array
		renderer_line_buf_t* arr_line_buf = get_line_buf(&ren->line_buf, i);

		// init the line buf that was retrive from array
		arr_line_buf->ansi_esc = NULL;
		fix_string_make(&arr_line_buf->buf, ren->w);
		fix_string_fill(&arr_line_buf->buf, ' ');
	}
}

void ui_renderer_make(ui_renderer_t* ren, u16 w, u16 h) {
	ren->w = w;
	ren->h = h;
	make_line_buf(ren);
}

void ui_renderer_clear(ui_renderer_t* ren) {
	for (u16 i = 0; i < array_size(&ren->line_buf); i++) {
		fix_string_fill(&(get_line_buf(&ren->line_buf, i))->buf, ' ');
	}
}

static void edit_line_buf(
	ui_renderer_t* ren,
	const char* str,
	u16 x,
	u16 y,
	const char* ansi_esc
	) {
	renderer_line_buf_t* line_buf = get_line_buf(&ren->line_buf, y);
	u16 char_i_counter = 0;

	for (u16 i = x; char_i_counter < strlen(str); i++) {
		fix_string_set_i(&line_buf->buf, str[char_i_counter], i);
		char_i_counter++;
	}

	line_buf->ansi_esc = ansi_esc;
}

static inline u16 calc_center_align_x_pos(ui_renderer_t* ren, usize label_len) {
	return (u16)floor(((f32)ren->w / 2.f) - ((f32)label_len / 2.f));
}

static void render_header(ui_renderer_t* ren, ui_head_component_t* comp) {
	u16 x = calc_center_align_x_pos(ren, strlen(comp->label));
	edit_line_buf(ren, comp->label, x, 0, ANSI_ESC_BLUE_B);
}

static inline ui_opt_component_t* get_option_component(vec_t* comps, u16 i) {
	return VEC_GET(comps, ui_opt_component_t, i);
}

static void render_options(ui_renderer_t* ren, vec_t* comps) {
	for (u16 i = 0; i < vec_size(comps); i++) {
		ui_opt_component_t* comp = get_option_component(comps, i);

		edit_line_buf(
			ren,
			comp->label,
			0,
			comp->line,
			ui_selector_on(comp) ?
				ANSI_ESC_WHITE_B :
				NULL);
	}
}

static inline ui_text_component_t* get_text_component(vec_t* comps, u16 i) {
	return VEC_GET(comps, ui_text_component_t, i);
}

static void resolve_text_x_pos(ui_renderer_t* ren, ui_text_component_t* comp) {
	if (comp->flags & ALIGN_CENTER || comp->flags == ALIGN_CENTER) {
		comp->x_pos = calc_center_align_x_pos(ren, strlen(comp->label));
	} else if (comp->flags & ALIGN_RIGHT || comp->flags == ALIGN_RIGHT) {
		comp->x_pos = (ren->w) - strlen(comp->label);
	} else if (comp->flags & ALIGN_LEFT || comp->flags == ALIGN_LEFT) {
		comp->x_pos = 0;
	} else {
		// if no flag is set standard alignment is left
		comp->x_pos = 0;
	}

	comp->x_pos_calc = TRUE;
}

static void render_text(ui_renderer_t* ren, vec_t* comps) {
	for (u16 i = 0; i < vec_size(comps); i++) {
		ui_text_component_t* comp = get_text_component(comps, i);
	
		// avoid calculating the position multiple times
		if (!comp->x_pos_calc) {
			resolve_text_x_pos(ren, comp);
		}

		edit_line_buf(ren, comp->label, comp->x_pos, comp->line, comp->color);
	}
} 

void ui_render_container(ui_renderer_t* ren, ui_container_t* con) {
	render_header(ren, &con->header);
	render_options(ren, &con->option);
	render_text(ren, &con->text);
}

void ui_renderer_draw(ui_renderer_t* ren) {
	for (u16 i = 0; i < array_size(&ren->line_buf); i++) {
		renderer_line_buf_t* line_buf = get_line_buf(&ren->line_buf, i);

		printf(
			"%s%s%s\n",
			(line_buf->ansi_esc == NULL) ? 
				"" :
				line_buf->ansi_esc,
			fix_string_get(&line_buf->buf),
			ANSI_ESC_RESET);
	}

	printf(ANSI_ESC_CURSOR_HOME);
}
