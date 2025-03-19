#include "gui.h"
#include "../network/client/client.h"

#include <keyboardio.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT WIDTH / 2 

#define FOOTER_DEFAULT_TEXT "r to return, q to quit"
#define FOOTER_RETURN_TEXT_ONLY "r to return"
#define FOOTER_QUIT_TEXT_ONLY "q to quit"

#define CONTAINER_ADD_FOOTER(CON, TEXT)                                       \
	do {                                                                      \
	ui_container_add_text(                                                    \
		CON,                                                                  \
		ui_text_component_new(                                                \
			TEXT,                                                             \
			FOOTER | ALIGN_RIGHT | COLOR_B | COLOR_GREEN));                   \
	} while (0)\

struct gui {
	bool running;
	bool should_update;

	ui_renderer_t renderer;

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;
	ui_trig_t quit_trig;
	ui_trig_t return_trig;

	ui_container_t main_con;

	ui_menu_t menu;
} static gui;

typedef struct gui gui_t;

static inline bool up_trig_func(void* arg) {
	return (kbio_ch == 'w');
}

static inline bool down_trig_func(void* arg) {
	return (kbio_ch == 's');
}

static inline bool selc_trig_func(void* arg) {
	return (kbio_ch == 'e');
}

static inline bool quit_trig_func(void* arg) {
	return (kbio_ch == 'q');
}

static inline bool return_trig_func(void* arg) {
	return (kbio_ch == 'r');
}

static void init_trigs() {
	ui_trig_make(&gui.up_trig, up_trig_func, NULL);
	ui_trig_make(&gui.down_trig, down_trig_func, NULL);
	ui_trig_make(&gui.selc_trig, selc_trig_func, NULL);
	ui_trig_make(&gui.quit_trig, quit_trig_func, NULL);
	ui_trig_make(&gui.return_trig, return_trig_func, NULL);
}

static inline void change_menu(void* arg) {
	ui_container_t* con = (ui_container_t*)arg;

	ui_menu_set_current(&gui.menu, con);
}

static void render_container(ui_container_t* con) {
	ui_renderer_clear(&gui.renderer);
	ui_render_container(&gui.renderer, con);
	ui_renderer_draw(&gui.renderer);
}

static void handle_get_student_id(void* arg) {
	ui_input_t name;

	ui_input_make(&name);

	if (!ui_input_get("Name", &name)) {
		return;
	}

	vec_t id_buf;

	if (!cli_req_id_by_name(ui_input_get_buf(&name), &id_buf)) {
		UI_RENDERER_OUT(2, "Fail to get ID", NULL);
		return;
	}

	ui_container_t container;
	ui_container_make(&container);

	ui_container_set_header(&container, ui_head_component_new("Students ID"));

	for (u32 i = 0; i < vec_size(&id_buf); i++) {
		ui_container_add_text(
			&container,
			ui_text_component_new(
				fix_string_get(VEC_GET(&id_buf, fix_string_t, i)), NONE)); 
	}

	CONTAINER_ADD_FOOTER(&container, FOOTER_RETURN_TEXT_ONLY);
	render_container(&container);

	while (!ui_trig_check(&gui.return_trig)) {
		kbio_check_input_block();
	}

	ui_container_destroy(&container);
	cli_req_id_by_name_destroy(&id_buf);
}

static void init_main_con() {
	ui_container_t* container = &gui.main_con;
	
	ui_container_make(container);

	ui_container_set_header(container, ui_head_component_new("Welcome!"));
	ui_container_add_opt(
		container,
		ui_opt_component_new(
			"Get Student ID", ui_call_back_new(handle_get_student_id, NULL)));
	CONTAINER_ADD_FOOTER(container, FOOTER_QUIT_TEXT_ONLY);

	ui_container_set_selector(
		container,
		ui_selector_new(gui.up_trig, gui.down_trig, gui.selc_trig, container));
}

void gui_init() {
	ui_menu_make(&gui.menu);
	ui_renderer_make(&gui.renderer, WIDTH, HEIGHT);

	init_trigs();
	init_main_con();

	ui_menu_set_current(&gui.menu, &gui.main_con);
	gui.should_update = true;
	ui_renderer_ready();

	gui.running = true;
}

void gui_deinit() {
	gui.running = false;
	ui_renderer_unready();
}

bool gui_should_run() {
	return gui.running;
}

void gui_loop() {
	if (ui_trig_check(&gui.quit_trig)) {
		gui.running = false;
		return;
	}

	if (ui_trig_check(&gui.return_trig)) {
		ui_menu_return(&gui.menu);
		gui.should_update = true;
	}

	ui_container_t* current_con = ui_menu_get_current(&gui.menu);

	if (gui.should_update) {
		render_container(current_con);
	}

	kbio_check_input_block();
	gui.should_update = ui_container_loop(current_con);
}
