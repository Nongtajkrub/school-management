#include "option.h"

ui_opt_component_t ui_opt_component_make(
	const char* label, ui_call_back_t call_back) {
	return (ui_opt_component_t) {
		.label = label,
		.call_back = call_back
	};
}

