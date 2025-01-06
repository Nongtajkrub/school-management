#include "container.h"

void ui_container_init(ui_container_t* con) {
	// the component 0 always have to be the header
	con->component_count = 1;
	VEC_MAKE(&con->option, ui_opt_component_t);
	VEC_MAKE(&con->text, ui_text_component_t);
}

void ui_container_set_header(ui_container_t* con, ui_head_component_t comp) {
	con->header = comp;
}

void ui_container_add_opt(ui_container_t* con, ui_opt_component_t* comp) {
	// if this is the first option component added
	if (vec_size(&con->option) == 0) {
		comp->selc_on = TRUE;
	}

	comp->line = con->component_count;
	vec_push(&con->option, comp);
	con->component_count++;
}

void ui_container_add_text(ui_container_t* con, ui_text_component_t* comp) {
	comp->line = con->component_count;
	vec_push(&con->text, comp);
	con->component_count++;
}

void ui_container_mk_and_set_header(ui_container_t* con, const char* label) {
	ui_head_component_t comp;
	ui_head_component_make(&comp, label);

	ui_container_set_header(con, comp);
}

void ui_container_mk_and_add_opt(
	ui_container_t* con,
	const char* label,
	void_func func,
	void* arg
	) {
	ui_call_back_t call_back;
	ui_call_back_make(&call_back, func, arg);

	ui_opt_component_t comp;
	ui_opt_component_make(&comp, label, call_back);

	ui_container_add_opt(con, &comp);
}

void ui_container_mk_and_add_text(
	ui_container_t* con,
	const char* label,
	ui_text_component_flags_t flags
	) {
	ui_text_component_t comp;
	ui_text_component_make(&comp, label, flags);

	ui_container_add_text(con, &comp);
}
