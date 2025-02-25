#pragma once

#include "gui_sys/gui_sys.h"

#include <type.h>

void gui_init();
void gui_deinit();

bool gui_should_run();
void gui_loop();
