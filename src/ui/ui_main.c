#include "ui/ui_all.h"

#include <unistd.h>

bool up_trig_func(void* arg) {
	return TRUE;
}

bool down_trig_func(void* arg) {
	return TRUE;
}

bool selc_trig_func(void* arg) {
	return TRUE;
}

void ui_main() {
	ui_renderer_t renderer;
	ui_renderer_make(&renderer, 20, 10);

	ui_component_t comp1;
	ui_component_make(&comp1, CENTERX | TOP ,"Welcome!!", &renderer);

	ui_component_t comp2;
	ui_component_make(&comp2, LEFT | OPTION, "Option1", &renderer);
	ui_component_posy(&comp2, 1);

	ui_control_trig_t up_trig;
	ui_control_trig_make(&up_trig, up_trig_func, NULL);

	ui_control_trig_t down_trig;
	ui_control_trig_make(&down_trig, down_trig_func, NULL);

	ui_control_trig_t selc_trig;
	ui_control_trig_make(&selc_trig, selc_trig_func, NULL);

	ui_control_t control;
	ui_control_make(&control, up_trig, down_trig, selc_trig);

	ui_menu_t menu;
	ui_menu_make(&menu, control, 2, &comp1, &comp2);

	ui_render_menu(&renderer, &menu);
	//ui_renderer_draw(&renderer);
	
	while (TRUE) {
		switch (ui_control_event(&control)) {
		case NO_EVENT:
			break;
		case UP_TRIG:
			printf("Up!\n");
			break;
		case DOWN_TRIG:
			printf("Down!\n");
			break;
		case SELC_TRIG:
			printf("Selc!\n");
			break;
		}

		sleep(1);
	}
}
