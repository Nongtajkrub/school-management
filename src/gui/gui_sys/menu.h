#pragma once

#include "container.h"

#include <vector.h>

typedef vec_t ui_menu_t;

static inline void ui_menu_make(ui_menu_t* menu) {
	VEC_MAKE(menu, ui_container_t);
}

static inline void ui_menu_set_current(ui_menu_t* menu, ui_container_t* con) {
	vec_push(menu, con);
}

static inline ui_container_t* ui_menu_get_current(ui_menu_t* menu) {
	return VEC_BACK(menu, ui_container_t); 
}

static inline void ui_menu_return(ui_menu_t* menu) {
	vec_pop_back(menu);
}
