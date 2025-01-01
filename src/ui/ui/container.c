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
	comp->line = con->component_count;
	vec_push(&con->option, comp);
	con->component_count++;
}

void ui_container_add_text(ui_container_t* con, ui_text_component_t* comp) {
	comp->line = con->component_count;
	vec_push(&con->option, comp);
	con->component_count++;
}
