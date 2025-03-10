#pragma once

#include <type.h>

typedef struct {
	const char* label;
} ui_head_component_t;

void ui_head_component_make(ui_head_component_t* comp, const char* label);
ui_head_component_t ui_head_component_new(const char* label);
