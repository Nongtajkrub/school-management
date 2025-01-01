// TODO: figure out keyboard input

#include "ui/ui_all.h"

bool temp(void* arg) {
	return TRUE;
}

void ui_main() {
	ui_renderer_t renderer;
	ui_renderer_make(&renderer, 20, 10);

	ui_component_t header;
	ui_component_make(&header, HEADER, "Welcome!");
	ui_component_t option1;
	ui_component_make(&option1, OPTION | CENTERX, "Settings");
	ui_component_t option2;
	ui_component_make(&option2, OPTION | CENTERX, "About");

	ui_control_trig_t trig;
	ui_control_trig_make(&trig, temp, NULL);

	ui_control_t control;
	ui_control_make(&control, trig, trig, trig);

	ui_menu_t menu;
	if (!ui_menu_make(&menu, control, &renderer, 3, &header, &option1, &option2)) {
		printf("Invalid menu!\n");
	}

	ui_render_menu(&renderer, &menu);
	ui_renderer_draw(&renderer);
}
