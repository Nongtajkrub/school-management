#pragma once

#include "gui_sys/gui_sys.h"

#include <type.h>

typedef struct {
	bool running;
	bool should_update;

	ui_renderer_t renderer;

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	// container ids
	u16 main_cid;
	u16 get_student_id_cid;

	ui_container_group_t container_group;
} gui_t;

void gui_init(gui_t* gui);
void gui_deinit();

void gui_loop(gui_t* gui);
