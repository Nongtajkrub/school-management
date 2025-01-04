#include "selector.h"
#include "container.h"

static inline ui_opt_component_t* get_opt_on_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on);
}

static inline ui_opt_component_t* get_opt_above_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on - 1);
}

static inline ui_opt_component_t* get_opt_below_selc(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, selc->on + 1);
}

static inline ui_opt_component_t* get_first_opt(ui_selector_t* selc) {
	return VEC_GET(selc->opt_components, ui_opt_component_t, 0);
}

void ui_selector_reset(ui_selector_t* selc) {
	// change selc_on on currently selected option
	// to false but the first option to true
	(get_opt_on_selc(selc))->selc_on = FALSE;
	(get_first_opt(selc))->selc_on = TRUE;
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

static void selc_up(ui_selector_t* selc) {
	if (selc->on == 0) {
		return;
	}

	// change selc_on on currently selected option
	// to false but the one above to true
	(get_opt_on_selc(selc))->selc_on = FALSE;
	(get_opt_above_selc(selc))->selc_on = TRUE;
	selc->on--;
 }

static void selc_down(ui_selector_t* selc) {
	if (selc->on >= vec_size(selc->opt_components) - 1) {
		return;
	}

	// change selc_on on currently selected option
	// to false but the one below to true
	(get_opt_on_selc(selc))->selc_on = FALSE;
	(get_opt_below_selc(selc))->selc_on = TRUE;
	selc->on++;
}

static void handle_selc(ui_selector_t* selc) {
	ui_opt_component_t* opt = get_opt_on_selc(selc);
	opt->call_back.func(opt->call_back.arg);
}

// return true if a selector update occure
bool ui_selector_loop(ui_selector_t* selc) {
	if (selc->up_trig.func(selc->up_trig.arg)) {
		selc_up(selc);
		return TRUE;
	} 

	if (selc->down_trig.func(selc->down_trig.arg)) {
		selc_down(selc);
		return TRUE;
	} 

	if (selc->selc_trig.func(selc->selc_trig.arg)) {
		handle_selc(selc);
		return TRUE;
	}

	return FALSE;
}
