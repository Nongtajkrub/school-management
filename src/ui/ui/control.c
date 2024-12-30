#include "control.h"

void ui_control_trig_make(
	ui_control_trig_t* trig,
	bool_func func,
	void* arg
	) {
	trig->func = func;
	trig->arg = arg;
}

void ui_control_make(
	ui_control_t* con,
	ui_control_trig_t up_trig,
	ui_control_trig_t down_trig,
	ui_control_trig_t selc_trig
	) {
	con->up_trig = up_trig;
	con->down_trig = down_trig;
	con->selc_trig = selc_trig;
}

ui_control_e_t ui_control_event(ui_control_t* con) {
	if (con->up_trig.func(con->up_trig.arg)) {
		return UP_TRIG;
	} else if (con->down_trig.func(con->down_trig.arg)) {
		return DOWN_TRIG;
	} else if (con->selc_trig.func(con->selc_trig.arg)) {
		return SELC_TRIG;
	}

	return NO_EVENT;
}
