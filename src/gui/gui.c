#include "gui_sys/container.h"
#include "gui_sys/renderer.h"

#include <keyboardio.h>

void void_temp(void* arg) {
	;;
}

bool up_trig_func(void* arg) {
	return (kbio_ch == 'w');
}

bool down_trig_func(void* arg) {
	return (kbio_ch == 's');
}

bool selc_trig_func(void* arg) {
	return (kbio_ch == 'e');
}

void ui_main() {
	ui_call_back_t temp_call_back;

	ui_call_back_make(&temp_call_back, void_temp, NULL);

	ui_head_component_t header;
	ui_opt_component_t opt1;
	ui_opt_component_t opt2;
	ui_opt_component_t opt3;
	ui_text_component_t text;

	ui_head_component_make(&header, "Welcom!");
	ui_opt_component_make(&opt1, "Settings", temp_call_back);
	ui_opt_component_make(&opt2, "Credit", temp_call_back);
	ui_opt_component_make(&opt3, "Testing", temp_call_back);
	ui_text_component_make(&text, "'q' to exit", ALIGN_RIGHT | COLOR_B | COLOR_GREEN);

	ui_container_t container;

	ui_container_init(&container);
	ui_container_set_header(&container, header);
	ui_container_add_opt(&container, &opt1);
	ui_container_add_opt(&container, &opt2);
	ui_container_add_opt(&container, &opt3);
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

	ui_renderer_make(&renderer, 100, 50);
	ui_renderer_ready();

	while (TRUE) {
		kbio_check_input();
	
		if (kbio_ch == 'q') {
			break;
		}

		if (ui_container_loop(&container)) {
			ui_render_container(&renderer, &container);
			ui_renderer_draw(&renderer);
		}
	}

	ui_renderer_unready();
}
