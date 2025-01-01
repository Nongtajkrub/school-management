#include "component.h"

void ui_text_component_make(
	ui_text_component_t* comp,
	const char* label,
	ui_component_align_t align
	) {
	comp->align = align;
	comp->label = label;
}
