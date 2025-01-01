#include "option.h"

void ui_opt_component_make(
	ui_opt_component_t* comp,
	const char* label,
	void_func call_back
	) {
	comp->label = label;
	comp->selc_on = FALSE;
	comp->call_back = call_back;
}

