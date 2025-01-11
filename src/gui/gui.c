#include "gui_sys/gui_sys.h"

#include <keyboardio.h>
#include <unistd.h>

#define MAKE_AND_ADD_SELECTOR(CONTAINER_NAME)                                 \
	do {                                                                      \
		ui_container_mk_and_set_selector(                                     \
			&CONTAINER_NAME,                                                  \
			gui->up_trig,                                                     \
			gui->down_trig,                                                   \
			gui->selc_trig);                                                  \
	} while(0); 

#define MAKE_CONTAINER_BEGIN(CONTAINER_NAME)                                  \
	ui_container_t CONTAINER_NAME;                                            \
	ui_container_init(&CONTAINER_NAME)

#define MAKE_CONTAINER_END(CONTAINER_NAME, CONTAINER_ID)                      \
	CONTAINER_ID =                                                            \
		ui_container_group_add(                                               \
			&gui->container_group,                                            \
			&CONTAINER_NAME);                                                 \
	//ui_container_uninit(&CONTAINER_NAME);                                   \

#define WIDTH 40
#define HEIGHT 20

void call_back(void* arg) {
	;;
}

static u16 current_container_id = 0;

typedef struct {
	bool running;
	bool should_update;

	ui_renderer_t renderer;

	ui_trig_t up_trig;
	ui_trig_t down_trig;
	ui_trig_t selc_trig;

	// container ids
	u16 main_cid;
	u16 request_balance_cid;
	u16 request_age_cid;

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
	current_container_id = *(u16*)arg;
}

static void make_container_main(gui_t* gui) {
	MAKE_CONTAINER_BEGIN(con);

	ui_container_mk_and_set_header(&con, "Welcome!");
	ui_container_mk_and_add_opt(
		&con,
		"Request Balance",
		change_container,
		&gui->request_balance_cid);
	ui_container_mk_and_add_opt(
		&con,
		"Request Age",
		change_container,
		&gui->request_age_cid);
	MAKE_AND_ADD_SELECTOR(con);

	MAKE_CONTAINER_END(con, gui->main_cid);
	current_container_id = gui->main_cid;
}

static void make_container_request_balance(gui_t* gui) {
	MAKE_CONTAINER_BEGIN(con);

	ui_container_mk_and_set_header(&con, "Request Balance");
	ui_container_mk_and_add_opt(&con, "Search By Name", call_back, NULL);
	MAKE_AND_ADD_SELECTOR(con);

	MAKE_CONTAINER_END(con, gui->request_balance_cid);
}

static void make_containers_request_age(gui_t* gui) {
	MAKE_CONTAINER_BEGIN(con);
	
	ui_container_mk_and_set_header(&con, "Request Age");
	ui_container_mk_and_add_opt(&con, "Search By Name", call_back, NULL);
	MAKE_AND_ADD_SELECTOR(con);

	MAKE_CONTAINER_END(con, gui->request_age_cid);
}

static void make_containers(gui_t* gui) {
	ui_container_group_init(&gui->container_group);

	make_container_main(gui);
	make_container_request_balance(gui);
	make_containers_request_age(gui);
}

static void init(gui_t* gui) {
	make_renderer(gui);
	make_trig(gui);
	make_containers(gui);

	//ui_renderer_ready();

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

	if (gui->should_update) {
		ui_renderer_clear(&gui->renderer);
		ui_render_container(&gui->renderer, current_container);
		ui_renderer_draw(&gui->renderer);
	}

	gui->should_update = ui_container_loop(current_container);
}

void gui_main() {
	gui_t gui;
	
	init(&gui);

	while (gui.running) {
		loop(&gui);
	}

	deinit();
}
