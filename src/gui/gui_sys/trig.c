#include "trig.h"

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg) {
	trig->func = func;
	trig->arg = arg;
}

ui_call_back_t ui_call_back_make(void_func func, void* arg) {
	return (ui_call_back_t) {
		.func = func,
		.arg = arg
	};
}
