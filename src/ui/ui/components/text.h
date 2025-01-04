#pragma once

#include <type.h>

typedef enum : u16 {
	NONE = 0,

	// alignment
	ALIGN_CENTER = 1 << 0,
	ALIGN_RIGHT  = 1 << 1,
	ALIGN_LEFT   = 1 << 2,

	// colors setting
	COLOR_B   = 1 << 3,
	COLOR_F   = 1 << 4,

	// colors
	COLOR_BLACK   = 1 << 5,
	COLOR_RED     = 1 << 6,
	COLOR_GREEN   = 1 << 7,
	COLOR_YELLOW  = 1 << 8,
	COLOR_BLUE    = 1 << 9,
	COLOR_MAGENTA = 1 << 10,
	COLOR_CYAN    = 1 << 11,
	COLOR_WHITE   = 1 << 12
} ui_text_component_flags_t;

typedef struct {
	const char* label;
	u16 line;

	ui_text_component_flags_t flags;

	bool x_pos_calc;
	u16 x_pos;

	const char* color;
} ui_text_component_t;

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label,
	ui_text_component_flags_t flags
	);
