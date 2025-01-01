#pragma once

#include <type.h>

typedef enum ui_flag : u16 {
	NO_FLAG = 0,

	// alignment
	CENTERX = 1 << 0,
	LEFT    = 1 << 1,
	RIGHT   = 1 << 2,

	// type
	HEADER = 1 << 3,
	OPTION = 1 << 4,
	TEXT   = 1 << 5
} ui_flag_t;

struct position {
	u16 x,
	   	y;
};

typedef struct ui_component {
	ui_flag_t flags;

	struct position pos;
	const char* label;

	// additional data for component
	struct {
		usize size;
		void* data;
	} data;
} ui_component_t;

void ui_component_make(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label
	);

void ui_component_pos(ui_component_t* comp, u16 x, u16 y);

static inline void ui_component_posx(ui_component_t* comp, u16 x) {
	comp->pos.x = x;
}

static inline void ui_component_posy(ui_component_t* comp, u16 y) {
	comp->pos.y = y;
}
