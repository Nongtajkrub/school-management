// TODO: test the selector

#pragma once

#include "components/option.h"
#include "trig.h"

typedef struct ui_container ui_container_t;

#include <type.h>
#include <vector.h>

typedef struct {
	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	// which option component is the selector on
	u16 on;
	
	vec_t* opt_components;
} ui_selector_t;

void ui_selector_make(
	ui_selector_t* selc,
	ui_trig_t up_trig,
	ui_trig_t down_trig,
	ui_trig_t selc_trig,
	ui_container_t* con
	);

void ui_selector_reset(ui_selector_t* selc);

// return the option component that was selected
// if none were selected return NULL
ui_opt_component_t* ui_selector_loop(ui_selector_t* selc);

static inline bool ui_selector_on(ui_opt_component_t* comp) {
	return comp->selc_on;
}
