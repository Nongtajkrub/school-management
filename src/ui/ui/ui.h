#pragma once

#include <type.h>
#include <vector.h>

typedef enum : u16 {
	NONE   = 0x00,
	CENTER = 0x01,
	HEADER = 0x02,
	OPTION = 0x04,
	TEXT   = 0x08
} ui_flag_t;

typedef struct {
	ui_flag_t flags;

	// component position
	struct {
		u16 x,
			y;
	} pos;

	const char* label;

	// additional data for component
	struct {
		usize size;
		void* data;
	} data;
} ui_component_t;

typedef struct {
	u16 id;

	vec_t components;
} ui_menu_t;

void ui_make_component(
	ui_component_t* comp,
	ui_flag_t flags,
	const char* label
	);

void ui_make_menu(ui_menu_t* menu, int c, ...);
