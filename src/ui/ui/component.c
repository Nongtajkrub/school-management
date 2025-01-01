#include "component.h"

#include <stdarg.h>

void ui_component_make(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label
	) {
	comp->flags = flags;
	comp->label = label;

	// postion will be calculate later
	ui_component_pos(comp, 0, 0);

}

void ui_component_pos(ui_component_t* comp, u16 x, u16 y) {
	comp->pos.x = x;
	comp->pos.y = y;
}
