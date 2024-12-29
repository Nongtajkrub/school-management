#include "ui/ui_all.h"

#include <time.h>

void ui_main() {
	ui_renderer_t renderer;
	ui_renderer_make(&renderer, 20, 10);

	ui_component_t comp1;
	ui_component_make(&comp1, CENTERX | TOP ,"Welcome!!", &renderer);
	ui_component_t comp2;
	ui_component_make(&comp2, CENTERX | BOTTOM ,"Welcome Again!!", &renderer);

	ui_menu_t menu;
	ui_menu_make(&menu, 2, &comp1, &comp2);

	ui_render_menu(&renderer, &menu);
	ui_renderer_draw(&renderer);
}
