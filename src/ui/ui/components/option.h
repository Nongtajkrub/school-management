#pragma once

#include <type.h>

typedef struct ui_opt_component {
	const char* label;
	u16 line;

	bool selc_on;
	void_func call_back;
} ui_opt_component_t;

void ui_opt_component_make(
	ui_opt_component_t* comp,
	const char* label,
	void_func call_back
	);
