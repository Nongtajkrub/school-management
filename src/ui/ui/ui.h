#pragma once

// forward declear for renderer.h
typedef struct ui_renderer ui_renderer_t;

#include <type.h>
#include <vector.h>

typedef enum ui_flag : u16 {
	NONE = 0,

	// alignment
	CENTERX = 1 << 0,
	CENTERY = 1 << 1,
	LEFT    = 1 << 2,
	RIGHT   = 1 << 3,
	TOP     = 1 << 4,
	BOTTOM  = 1 << 5,

	// type
	HEADER = 1 << 6,
	OPTION = 1 << 7,
	TEXT   = 1 << 8
} ui_flag_t;

struct position {
	u8 x,
	   y;
};

typedef struct ui_component {
	ui_flag_t flags;

	// component position
	struct position pos;

	const char* label;

	// additional data for component
	struct {
		usize size;
		void* data;
	} data;
} ui_component_t;

typedef struct ui_menu {
	u16 id;

	vec_t components;
} ui_menu_t;

void ui_component_make(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label,
	ui_renderer_t* ren
	);

void ui_menu_make(ui_menu_t* menu, int c, ...);
