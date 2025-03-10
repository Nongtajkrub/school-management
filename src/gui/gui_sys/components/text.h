#pragma once

#include <type.h>

typedef enum : u16 {
	NONE = 0,

	// alignment
	ALIGN_CENTER  = 1 << 0,
	ALIGN_RIGHT   = 1 << 1,
	ALIGN_LEFT    = 1 << 2,

	// option
	FOOTER        = 1 << 3,

	// colors setting
	COLOR_B       = 1 << 4,
	COLOR_F       = 1 << 5,

	// colors
	COLOR_BLACK   = 1 << 6,
	COLOR_RED     = 1 << 7,
	COLOR_GREEN   = 1 << 8,
	COLOR_YELLOW  = 1 << 9,
	COLOR_BLUE    = 1 << 10,
	COLOR_MAGENTA = 1 << 11,
	COLOR_CYAN    = 1 << 12,
	COLOR_WHITE   = 1 << 13
} ui_text_component_flags_t;

typedef struct {
	const char* label;
	u16 line;

	ui_text_component_flags_t flags;

	bool pos_resolved;
	u16 x_pos;

	const char* color;
} ui_text_component_t;

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label, ui_text_component_flags_t flags);

ui_text_component_t ui_text_component_new(
	const char* label, ui_text_component_flags_t flags);
