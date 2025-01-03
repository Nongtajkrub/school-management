#include "ui/container.h"
#include "ui/renderer.h"

#include <unistd.h>

void void_temp(void* arg) {
	;;
}

bool up_trig_func(void* arg) {
	return FALSE;
}

bool down_trig_func(void* arg) {
	return TRUE;
}

bool selc_trig_func(void* arg) {
	return TRUE;
}

void ui_main() {
	ui_head_component_t header;
	ui_opt_component_t opt1;
	ui_opt_component_t opt2;
	ui_text_component_t text;

	ui_head_component_make(&header, "Welcom!");
	ui_opt_component_make(&opt1, "Settings", void_temp);
	ui_opt_component_make(&opt2, "Credit", void_temp);
	ui_text_component_make(&text, "END", LEFT);

	ui_container_t container;

	ui_container_init(&container);
	ui_container_set_header(&container, header);
	ui_container_add_opt(&container, &opt1);
	ui_container_add_opt(&container, &opt2);
	ui_container_add_text(&container, &text);

	ui_selector_t selector;
	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	ui_trig_make(&up_trig, up_trig_func, NULL);
	ui_trig_make(&down_trig, down_trig_func, NULL);
	ui_trig_make(&selc_trig, selc_trig_func, NULL);
	ui_selector_make(&selector, up_trig, down_trig, selc_trig, &container);

	ui_container_set_selector(&container, selector);

	ui_renderer_t renderer;

	ui_renderer_make(&renderer, 20, 10);

	while (TRUE) {
		ui_render_container(&renderer, &container);
		ui_renderer_draw(&renderer);
		ui_container_loop(&container);

		sleep(1);
	}
}
