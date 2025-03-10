#include "header.h"

void ui_head_component_make(ui_head_component_t* comp, const char* label) {
	comp->label = label;
}

ui_head_component_t ui_head_component_new(const char* label) {
	return (ui_head_component_t) {
		.label = label
	};
}
