#include "ui/ui_all.h"

void ui_main() {
	ui_component_t comp;

	ui_component_make(&comp, CENTER | HEADER, "Welcome!");

	ui_menu_t menu;

	ui_menu_make(&menu, 1, &comp);

	ui_renderer_t ren;

	ui_renderer_make(&ren, 15, 15);
	ui_render_menu(&ren, &menu);
	ui_renderer_draw(&ren);
}
