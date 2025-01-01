#pragma once

// forward declear for renderer.h
typedef struct ui_renderer ui_renderer_t;

#include "control.h"

#include <type.h>
#include <vector.h>

typedef struct ui_menu {
	ui_control_t control;

	vec_t components;
} ui_menu_t;

bool ui_menu_make(
	ui_menu_t* menu,
	ui_control_t control,
	ui_renderer_t* ren,
	int c, ...
	);
