#pragma once

#include "components/component.h"
#include "selector.h"

#include <vector.h>

typedef struct ui_container {
	u16 component_count;

	ui_head_component_t header;
	vec_t option;
	vec_t text;

	ui_selector_t selc;
} ui_container_t;

void ui_container_init(ui_container_t* con);

void ui_container_set_header(ui_container_t* con, ui_head_component_t comp);
void ui_container_add_opt(ui_container_t* con, ui_opt_component_t* comp);
void ui_container_add_text(ui_container_t* con, ui_text_component_t* comp);

static inline void ui_container_set_selector(
	ui_container_t* con,
	ui_selector_t selc
	) {
	con->selc = selc;
}

// return TRUE if and update occure
static inline bool ui_container_loop(ui_container_t* con) {
	return ui_selector_loop(&con->selc);
}

void ui_container_mk_and_set_header(ui_container_t* con, const char* label);
void ui_container_mk_and_add_opt(
	ui_container_t* con,
	const char* label,
	void_func call_back,
	void* arg
	);
void ui_container_mk_and_add_text(
	ui_container_t* con,
	const char* label,
	ui_text_component_flags_t flags
	);
