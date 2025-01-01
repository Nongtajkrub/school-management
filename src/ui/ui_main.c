#include "ui/container.h"
#include "ui/renderer.h"

void temp(void* arg) {
	;;
}

void ui_main() {
	ui_head_component_t header;
	ui_opt_component_t opt;
	ui_text_component_t text;

	ui_head_component_make(&header, "Welcom!");
	ui_opt_component_make(&opt, "Settings", temp);
	ui_text_component_make(&text, "END", CENTER);

	ui_container_t container;

	ui_container_init(&container);
	ui_container_set_header(&container, header);
	ui_container_add_opt(&container, &opt);
	ui_container_add_text(&container, &text);

	ui_renderer_t renderer;

	ui_renderer_make(&renderer, 20, 10);
	ui_render_container(&renderer, &container);
	ui_renderer_draw(&renderer);
}
