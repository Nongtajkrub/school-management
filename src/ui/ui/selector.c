#include "selector.h"
#include "container.h"

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg) {
	trig->func = func;
	trig->arg = arg;
}

void ui_selector_make(
	ui_selector_t* selc,
	ui_trig_t up_trig,
	ui_trig_t down_trig,
	ui_trig_t selc_trig,
	ui_container_t* con
	) {
	selc->up_trig = up_trig;
	selc->down_trig = down_trig;
	selc->selc_trig = selc_trig;
	selc->on = 0;
	selc->opt_components = &con->option;
}

static inline ui_opt_component_t* get_opt_on_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on);
}

static inline ui_opt_component_t* get_opt_above_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on - 1);
}

static inline ui_opt_component_t* get_opt_below_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on + 1);
}

static void selc_up(ui_selector_t* selc) {
	if (selc->on == 0) {
		return;
	}

	// change selc_on on currently selected option
	// to false but the one above to true
	(get_opt_on_selc(selc))->selc_on = FALSE;
	(get_opt_above_selc(selc))->selc_on = TRUE;
 }

static void selc_down(ui_selector_t* selc) {
	if (selc->on >= vec_size(selc->opt_components)) {
		return;
	}

	// change selc_on on currently selected option
	// to false but the one below to true
	(get_opt_on_selc(selc))->selc_on = FALSE;
	(get_opt_below_selc(selc))->selc_on = TRUE;
}

// return the option component that was selected
// if none were selected return NULL
ui_opt_component_t* ui_selector_loop(ui_selector_t* selc) {
	if (selc->up_trig.func(selc->up_trig.arg)) {
		selc_up(selc);
	} else if (selc->down_trig.func(selc->down_trig.arg)) {
		selc_down(selc);
	} else if (selc->selc_trig.func(selc->selc_trig.arg)) {
		return get_opt_on_selc(selc);
	}

	return NULL;
}
