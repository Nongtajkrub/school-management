#include "option.h"

void ui_opt_component_make(
	ui_opt_component_t* comp,
	const char* label,
	ui_call_back_t call_back
	) {
	comp->label = label;
	comp->selc_on = FALSE;
	comp->call_back = call_back;
}

