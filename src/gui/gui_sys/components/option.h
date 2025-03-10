#pragma once

#include "../trig.h"

#include <type.h>

typedef struct ui_opt_component {
	const char* label;
	u16 line;

	bool selc_on;
	ui_call_back_t call_back;
} ui_opt_component_t;

void ui_opt_component_make(
	ui_opt_component_t* comp, const char* label, ui_call_back_t call_back);

ui_opt_component_t ui_opt_component_new(
	const char* label, ui_call_back_t call_back);
