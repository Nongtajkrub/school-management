#include "trig.h"

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg) {
	trig->func = func;
	trig->arg = arg;
}

void ui_call_back_make(ui_call_back_t* call_back, void_func func, void* arg) {
	call_back->func = func;
	call_back->arg = arg;
}
