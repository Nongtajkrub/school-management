#include "trig.h"

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg) {
	trig->func = func;
	trig->arg = arg;
}
