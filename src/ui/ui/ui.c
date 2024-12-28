#include "ui.h"

#include <stdarg.h>

void ui_make_component(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label
	) {
	comp->flags = flags;

	// postion will be calculate later
	comp->pos.x = 0;
	comp->pos.y = 0;

	comp->label = label;
}

void ui_make_menu(ui_menu_t* menu, int c, ...) {
	static u16 menu_count = 0;

	va_list components;
	va_start(components, c);

	menu->id = menu_count;
	VEC_MAKE(&menu->components, ui_component_t);

	// add all component
	for (u16 i = 0; i < c; i++) {
		vec_push(&menu->components, (void*)va_arg(components, ui_component_t*));
	}
}
