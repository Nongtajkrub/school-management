#pragma once

#include <type.h>

typedef struct {
	const char* label;
} ui_head_component_t;

ui_head_component_t ui_head_component_make(const char* label);
