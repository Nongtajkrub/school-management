#include "gui_sys/container.h"
#include "gui_sys/renderer.h"

#include <keyboardio.h>

typedef struct {
	ui_renderer_t renderer;
	ui_container_t container;
} gui_t;

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

void gui_main() {
	ui_renderer_t renderer;
	ui_renderer_make(&renderer, 60, 30);

	ui_container_t container;
	ui_container_init(&container);

	ui_container_mk_and_set_header(&container, "Welcome!");
	ui_container_mk_and_add_opt(&container, "Settings", void_temp, NULL);
	ui_container_mk_and_add_opt(&container, "Credit", void_temp, NULL);
	ui_container_mk_and_add_text(&container, "'q' to quit", COLOR_B | COLOR_GREEN | ALIGN_RIGHT);

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	ui_trig_make(&up_trig, up_trig_func, NULL);
	ui_trig_make(&down_trig, down_trig_func, NULL);
	ui_trig_make(&selc_trig, selc_trig_func, NULL);

	ui_container_mk_and_set_selector(&container, up_trig, down_trig, selc_trig);

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
