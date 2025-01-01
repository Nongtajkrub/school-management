#include "menu.h"
#include "renderer.h"
#include "component.h"

#include <string.h>
#include <stdarg.h>
#include <math.h>

static inline void left_align_component(ui_component_t* comp) {
	comp->pos.x = 0;
}

static inline void right_align_component(
	ui_component_t* comp,
	ui_renderer_t* ren
	) {
	comp->pos.x = (ren->w - 1) - strlen(comp->label);
}

static inline void center_componet_x(ui_component_t* comp, ui_renderer_t* ren) {
	comp->pos.x = (u32)floor(
		(u32)((f32)(ren->w - 1) / 2.f) -
			(u32)floor((f32)strlen(comp->label) / 2.f)
		);
}

static inline bool is_valid_alignment(ui_component_t* comp) {
	return !(
		(comp->flags & LEFT && comp->flags & RIGHT) ||
		((comp->flags & RIGHT || comp->flags & LEFT) && comp->flags & CENTERX)
		);
}

static inline bool is_header(ui_component_t* comp) {
	return (comp->flags & HEADER || comp->flags == HEADER);
}

static inline bool is_option(ui_component_t* comp) {
	return (comp->flags & OPTION || comp->flags == OPTION);
}

static inline bool is_text(ui_component_t* comp) {
	return (comp->flags & TEXT || comp->flags == TEXT);
}

static bool align_comp(ui_component_t* comp, ui_renderer_t* ren, u32 i) {
	if (!is_valid_alignment(comp)) {
		return FALSE;
	}

	if (comp->flags & CENTERX || is_header(comp)) {
		center_componet_x(comp, ren);
	} else if (comp->flags & RIGHT) {
		right_align_component(comp, ren);
	} else if (comp->flags & LEFT || is_option(comp) || is_text(comp)) {
		left_align_component(comp);
	} else {
		return FALSE;
	}

	comp->pos.y = i;
	return TRUE;
}

static inline bool is_valid_header(ui_component_t* comp) {
	return !(comp->flags & OPTION || comp->flags & TEXT);
}

static bool handle_header(
	ui_component_t* comp,
	ui_renderer_t* ren,
	bool header_exist,
	u32 i
	) {
	if (header_exist || !is_valid_header(comp)) {
		return FALSE;
	}

	// check first if user want to manually align the component
	return align_comp(comp, ren, i);
}

static inline bool is_valid_option(ui_component_t* comp) {
	return !(comp->flags & HEADER || comp->flags & TEXT);
}

static bool handle_option(ui_component_t* comp, ui_renderer_t* ren, u32 i) {
	if (!is_valid_option(comp)) {
		return FALSE;
	}

	// check first if user want to manually align the component
	return align_comp(comp, ren, i);
}

static inline bool is_valid_text(ui_component_t* comp) {
	return !(comp->flags & HEADER || comp->flags & OPTION);
}

static bool handle_text(ui_component_t* comp, ui_renderer_t* ren, u32 i) {
	if (!is_valid_text(comp)) {
		return FALSE;
	}

	return align_comp(comp, ren, i);
}

static bool make_layout(vec_t* comps, ui_renderer_t* ren) {
	bool header_exist = FALSE;

	for (u32 i = 0; i < vec_size(comps); i++) {
		ui_component_t* comp = VEC_GET(comps, ui_component_t, i);

		// header
		if (is_header(comp)) {
			if (!handle_header(comp, ren, header_exist, i)) {
				return FALSE;
			}
			header_exist = TRUE;
			continue;
		}

		// option
		if (is_option(comp)) {
			if (!handle_option(comp, ren, i)) {
				return FALSE;
			}
			continue;
		}

		// text
		if (is_text(comp)) {
			if (!handle_text(comp, ren, i)) {
				return FALSE;
			}
			continue;
		}

		// if reach this point mean failure
		return FALSE;
	}

	return TRUE;
}

bool ui_menu_make(
	ui_menu_t* menu,
	ui_control_t control,
	ui_renderer_t* ren,
	int c, ...
	) {
	va_list components;
	va_start(components, c);

	menu->control = control;
	VEC_MAKE(&menu->components, ui_component_t);

	// add all component
	for (u16 i = 0; i < c; i++) {
		ui_component_t* comp = va_arg(components, ui_component_t*);

		// no component in menu
		if (comp == NULL) {
			return TRUE;
		}

		vec_push(&menu->components, (void*)comp);
	}

	return make_layout(&menu->components, ren);
}
