#pragma once

#include "components/component.h"
	
#include <vector.h>

typedef struct ui_container {
	u16 component_count;

	ui_head_component_t header;
	vec_t option;
	vec_t text;
} ui_container_t;

void ui_container_init(ui_container_t* con);

void ui_container_set_header(ui_container_t* con, ui_head_component_t comp);
void ui_container_add_opt(ui_container_t* con, ui_opt_component_t* comp);
void ui_container_add_text(ui_container_t* con, ui_text_component_t* comp);
