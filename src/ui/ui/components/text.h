#pragma once

#include <type.h>

typedef enum {
	CENTER,
	RIGHT,
	LEFT
} ui_component_align_t;

typedef struct {
	const char* label;
	u16 line;

	ui_component_align_t align;
} ui_text_component_t;

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label,
	ui_component_align_t align
	);
