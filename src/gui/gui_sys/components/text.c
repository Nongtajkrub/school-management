#include "component.h"

#include <ansi_ecs.h>

static inline const char* color_B_or_F(
	ui_text_component_flags_t flags,
	const char* color_b, const char* color_f) {
	return (flags & COLOR_B) ? color_b : color_f;
}

static const char* resolve_color_code(ui_text_component_flags_t flags) {
	if (flags & COLOR_BLACK || flags == COLOR_BLACK) {
		return color_B_or_F(flags, ANSI_ESC_BLACK_B, ANSI_ESC_BLACK_F);
	} else if (flags & COLOR_RED || flags == COLOR_RED) {
		return color_B_or_F(flags, ANSI_ESC_RED_B, ANSI_ESC_RED_F);
	} else if (flags & COLOR_GREEN || flags == COLOR_GREEN) {
		return color_B_or_F(flags, ANSI_ESC_GREEN_B, ANSI_ESC_GREEN_F);
	} else if (flags & COLOR_YELLOW || flags == COLOR_YELLOW) {
		return color_B_or_F(flags, ANSI_ESC_YELLOW_B, ANSI_ESC_YELLOW_F);
	} else if (flags & COLOR_BLUE || flags == COLOR_BLUE) {
		return color_B_or_F(flags, ANSI_ESC_BLUE_B, ANSI_ESC_BLUE_F);
	} else if (flags & COLOR_MAGENTA || flags == COLOR_MAGENTA) {
		return color_B_or_F(flags, ANSI_ESC_MAGENTA_B, ANSI_ESC_MAGENTA_F);
	} else if (flags & COLOR_CYAN || flags == COLOR_CYAN) {
		return color_B_or_F(flags, ANSI_ESC_CYAN_B, ANSI_ESC_CYAN_F);
	} else if (flags & COLOR_WHITE || flags == COLOR_WHITE) {
		return color_B_or_F(flags, ANSI_ESC_WHITE_B, ANSI_ESC_WHITE_F);
	} else {
		// if no flag are set
		return NULL;
	}
}

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label, ui_text_component_flags_t flags) {
	comp->label = fix_string_from(label);
	comp->flags = flags;
	comp->pos_resolved = false;
	comp->x_pos = 0;
	comp->color = resolve_color_code(flags);
}

ui_text_component_t ui_text_component_new(
	const char* label, ui_text_component_flags_t flags) {
	return (ui_text_component_t) {
		.label = fix_string_from(label),
		.flags = flags,
		.pos_resolved = false,
		.x_pos = 0,
		.color = resolve_color_code(flags)
	};
}
