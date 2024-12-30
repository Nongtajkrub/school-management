#include "ui.h"
#include "renderer.h"

#include <stdarg.h>
#include <math.h>

// width and height is beign subtract by one everytime 
// because string index start at 0
static struct position resolve_component_pos(
	ui_component_t* comp,
	ui_renderer_t* ren
	) {
	struct position pos = {
		.x = 0,
		.y = 0
	};

	// center x and center y alignment 
	if (comp->flags & CENTERX || comp->flags == CENTERX) {
		pos.x = (u32)floor(
			(u32)((f32)(ren->w - 1) / 2.f) -
				(u32)floor((f32)strlen(comp->label) / 2.f)
			);
	}
	if (comp->flags & CENTERY || comp->flags == CENTERY) {
		pos.y = (u32)floor((f32)(ren->h - 1) / 2.f);
	}

	// left and right alignment
	if (comp->flags & LEFT || comp->flags == LEFT) {
		pos.x = 0;
	} else if (comp->flags & RIGHT || comp->flags == RIGHT) {
		pos.x = (ren->w - 1) - strlen(comp->label);
	}

	// top and bottom alignment
	if (comp->flags & TOP || comp->flags == TOP) {
		pos.y = 0;
	} else if (comp->flags & BOTTOM || comp->flags == BOTTOM) {
		pos.y = ren->h - 1;
	}

	return pos;
}

void ui_component_make(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label,
	ui_renderer_t* ren
	) {
	comp->flags = flags;
	comp->label = label;

	// postion will be calculate later
	comp->pos = resolve_component_pos(comp, ren);
}

void ui_component_pos(ui_component_t* comp, u16 x, u16 y) {
	comp->pos.x = x;
	comp->pos.y = y;
}

void ui_menu_make(ui_menu_t* menu, ui_control_t control, int c, ...) {
	static u16 menu_count = 0;

	va_list components;
	va_start(components, c);

	menu->id = menu_count;
	VEC_MAKE(&menu->components, ui_component_t);

	// add all component
	for (u16 i = 0; i < c; i++) {
		ui_component_t* comp = va_arg(components, ui_component_t*);

		// no component in menu
		if (comp == NULL) {
			return;
		}

		vec_push(&menu->components, (void*)comp);
	}

	menu->control = control;
}
