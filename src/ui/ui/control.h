#pragma once

#include <type.h>

typedef enum : u16 {
	NO_EVENT,
	DOWN_TRIG,
	UP_TRIG,
	SELC_TRIG
} ui_control_e_t;

typedef struct {
	bool_func func;
	void* arg;
} ui_control_trig_t;

void ui_control_trig_make(
	ui_control_trig_t* trig,
	bool_func func,
	void* arg
	);

typedef struct ui_control {
	ui_control_trig_t up_trig;
	ui_control_trig_t down_trig;
	ui_control_trig_t selc_trig;
} ui_control_t;

void ui_control_make(
	ui_control_t* con,
	ui_control_trig_t up_trig,
	ui_control_trig_t down_trig,
	ui_control_trig_t selc_trig
	);

ui_control_e_t ui_control_event(ui_control_t* con);
