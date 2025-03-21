#pragma once

#include <type.h>

typedef struct {
	bool_func func;
	void* arg;
} ui_trig_t;

void ui_trig_make(ui_trig_t* trig, bool_func func, void* arg);

static inline bool ui_trig_check(ui_trig_t* trig) {
	return (trig->func(trig->arg));
}

typedef struct {
	void_func func;
	void* arg;
} ui_call_back_t;

void ui_call_back_make(ui_call_back_t* call_back, void_func func, void* arg);
ui_call_back_t ui_call_back_new(void_func func, void* arg);
