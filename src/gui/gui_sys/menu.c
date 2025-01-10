#include "menu.h"

u16 ui_container_group_add(ui_container_group_t* group, ui_container_t* con) {
	vec_push(group, con);
	u16 id = vec_size(group) - 1;

	// reassign the selector option components (dont delete)
	ui_container_t* vec_con = VEC_GET(group, ui_container_t, id);
	vec_con->selc.opt_components = &vec_con->option;

	return id;
}
