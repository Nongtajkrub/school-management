#include "option.h"

void ui_opt_component_make(
	ui_opt_component_t* comp, const char* label, ui_call_back_t call_back) {
	comp->label = label;
	comp->selc_on = false;
	comp->call_back = call_back;
}

ui_opt_component_t ui_opt_component_new(
	const char* label, ui_call_back_t call_back) {
	return (ui_opt_component_t) {
		.label = label,
		.call_back = call_back
	};
}

