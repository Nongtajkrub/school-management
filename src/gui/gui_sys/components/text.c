#include "component.h"

#include <ansi_ecs.h>

static inline void set_color(
	ui_text_component_t* comp,
	const char* color_b,
	const char* color_f
	) {
	comp->color = (comp->flags & COLOR_B) ? color_b : color_f;
} 

static void resolve_color_code(ui_text_component_t* comp) {
	if (comp->flags & COLOR_BLACK || comp->flags == COLOR_BLACK) {
		set_color(comp, ANSI_ESC_BLACK_B, ANSI_ESC_BLACK_F);
	} else if (comp->flags & COLOR_RED || comp->flags == COLOR_RED) {
		set_color(comp, ANSI_ESC_RED_B, ANSI_ESC_RED_F);
	} else if (comp->flags & COLOR_GREEN || comp->flags == COLOR_GREEN) {
		set_color(comp, ANSI_ESC_GREEN_B, ANSI_ESC_GREEN_F);
	} else if (comp->flags & COLOR_YELLOW || comp->flags == COLOR_YELLOW) {
		set_color(comp, ANSI_ESC_YELLOW_B, ANSI_ESC_YELLOW_F);
	} else if (comp->flags & COLOR_BLUE || comp->flags == COLOR_BLUE) {
		set_color(comp, ANSI_ESC_BLUE_B, ANSI_ESC_BLUE_F);
	} else if (comp->flags & COLOR_MAGENTA || comp->flags == COLOR_MAGENTA) {
		set_color(comp, ANSI_ESC_MAGENTA_B, ANSI_ESC_MAGENTA_F);
	} else if (comp->flags & COLOR_CYAN || comp->flags == COLOR_CYAN) {
		set_color(comp, ANSI_ESC_CYAN_B, ANSI_ESC_CYAN_F);
	} else if (comp->flags & COLOR_WHITE || comp->flags == COLOR_WHITE) {
		set_color(comp, ANSI_ESC_WHITE_B, ANSI_ESC_WHITE_F);
	} else {
		// if no flag are set
		comp->color = NULL;
	}
}

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label,
	ui_text_component_flags_t flags
	) {
	comp->label = label;
	comp->flags = flags;
	comp->x_pos_calc = FALSE;
	comp->x_pos = 0;
	resolve_color_code(comp);
}
