#include "gui.h"

#include <keyboardio.h>

#define WIDTH 40
#define HEIGHT WIDTH / 2 

struct gui {
	bool running;
	bool should_update;

	ui_renderer_t renderer;

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	ui_container_t main_con;
	ui_container_t get_student_id_con;

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

static void init_selector() {
	ui_trig_make(&gui.up_trig, up_trig_func, NULL);
	ui_trig_make(&gui.down_trig, down_trig_func, NULL);
	ui_trig_make(&gui.selc_trig, selc_trig_func, NULL);
}

static inline void change_menu(void* arg) {
	ui_container_t* con = (ui_container_t*)arg;

	ui_menu_set_current(&gui.menu, con);
}

static void init_main_con() {
	ui_container_t* con = &gui.main_con;
	
	ui_container_make(con);

	ui_container_mk_and_set_header(con, "Main Menu");
	ui_container_mk_and_add_opt(
		con, "Get Student ID", change_menu, &gui.get_student_id_con);
	ui_container_mk_and_set_selector(
		con, gui.up_trig, gui.down_trig, gui.selc_trig);
}

static void init_get_student_id_con() {
	ui_container_t* con = &gui.get_student_id_con;

	ui_container_make(con);

	ui_container_mk_and_set_header(con, "Get Studen ID");
	ui_container_mk_and_add_opt(con, "By Name", NULL, NULL);
	ui_container_mk_and_add_opt(con, "By Class", NULL, NULL);
	ui_container_mk_and_set_selector(
		con, gui.up_trig, gui.down_trig, gui.selc_trig);
}

void gui_init() {
	ui_menu_make(&gui.menu);
	ui_renderer_make(&gui.renderer, WIDTH, HEIGHT);

	init_selector();
	init_main_con();
	init_get_student_id_con();

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

static void render_container(ui_container_t* con) {
	ui_renderer_clear(&gui.renderer);
	ui_render_container(&gui.renderer, con);
	ui_renderer_draw(&gui.renderer);
}

void gui_loop() {
	if (kbio_ch == 'q') {
		gui_deinit();
		return;
	}

	ui_container_t* current_con = ui_menu_get_current(&gui.menu);

	if (gui.should_update) {
		render_container(current_con);
	}

	kbio_check_input_block();
	gui.should_update = ui_container_loop(current_con);
}
