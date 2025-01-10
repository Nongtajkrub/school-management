// TODO: fix selector causing segment fault 

#include "gui_sys/gui_sys.h"

#include <keyboardio.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

void call_back(void* arg) {
	;;
}

static u16 current_container_id = 0;

typedef struct {
	bool running;

	ui_renderer_t renderer;

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	u16 main_container_id;
	u16 req_balance_container_id;

	ui_container_group_t container_group;
} gui_t;

static inline void make_renderer(gui_t* gui) {
	ui_renderer_make(&gui->renderer, WIDTH, HEIGHT);
}

bool up_trig_func(void *arg) {
	return (kbio_ch == 'w');
}

bool down_trig_func(void *arg) {
	return (kbio_ch == 's');
}

bool selc_trig_func(void *arg) {
	return (kbio_ch == 'e');
}

static void make_trig(gui_t* gui) {
	ui_trig_make(&gui->up_trig, up_trig_func, NULL);
	ui_trig_make(&gui->down_trig, down_trig_func, NULL);
	ui_trig_make(&gui->selc_trig, selc_trig_func, NULL);
}

static void change_container(void* arg) {
	//printf("Change container\n");
	current_container_id = *(u16*)arg;

	//printf("arg -> %d\n", *(u16*)arg);
}

static void make_container_main(gui_t* gui) {
	ui_container_t con;
	ui_container_init(&con);

	ui_container_mk_and_set_header(&con, "Welcome!");
	ui_container_mk_and_add_opt(
		&con,
		"Request Balance",
		change_container,
		&gui->req_balance_container_id);
	ui_container_mk_and_add_opt(&con, "Request Age", call_back, NULL);
	ui_container_mk_and_set_selector(
		&con,
		gui->up_trig,
		gui->down_trig,
		gui->selc_trig);

	gui->main_container_id =
		ui_container_group_add(
			&gui->container_group,
			&con);

	current_container_id = gui->main_container_id;
}

static void make_container_req_balance(gui_t* gui) {
	ui_container_t con;
	ui_container_init(&con);

	ui_container_mk_and_set_header(&con, "Request Balance");
	ui_container_mk_and_add_opt(&con, "Search By Name", call_back, NULL);
	ui_container_mk_and_set_selector(
		&con,
		gui->up_trig,
		gui->down_trig,
		gui->selc_trig);

	gui->req_balance_container_id = 
		ui_container_group_add(
			&gui->container_group,
			&con);
}

static void make_containers(gui_t* gui) {
	ui_container_group_init(&gui->container_group);

	make_container_main(gui);
	make_container_req_balance(gui);
}

static void init(gui_t* gui) {
	make_renderer(gui);
	make_trig(gui);
	make_containers(gui);

	ui_renderer_ready();

	gui->running = TRUE;
}

static void deinit() {
	ui_renderer_unready();
}

static void loop(gui_t* gui) {
	kbio_check_input();

	if (kbio_ch == 'q') {
		gui->running = FALSE;
		return;
	}

	ui_container_t* current_container =
		ui_container_group_get(
			&gui->container_group,
			current_container_id);

	if (ui_container_loop(current_container)) {
		ui_renderer_clear(&gui->renderer);
		ui_render_container(&gui->renderer, current_container);
		ui_renderer_draw(&gui->renderer);
	}
}

void gui_main() {
	gui_t gui;
	
	init(&gui);

	while (gui.running) {
		loop(&gui);
	}

	deinit();
}
