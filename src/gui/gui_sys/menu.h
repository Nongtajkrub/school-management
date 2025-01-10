#pragma once

#include "container.h"
#include "selector.h"

#include <type.h>
#include <vector.h>

typedef vec_t ui_container_group_t;

static inline void ui_container_group_init(ui_container_group_t* group) {
	VEC_MAKE(group, ui_container_t);
}

// return menu id that was added
u16 ui_container_group_add(ui_container_group_t* group, ui_container_t* con);

static inline ui_container_t* ui_container_group_get(
	ui_container_group_t* group,
	u16 id
	){
	return VEC_GET(group, ui_container_t, id);
}
