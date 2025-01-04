#pragma once

#include <type.h>

typedef struct {
	bool_func func;
	void* arg;
} ui_trig_t;

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg);
