#include "gui_sys/gui_sys.h"

#include <keyboardio.h>
#include <unistd.h>

#define MAKE_AND_ADD_SELECTOR(CONTAINER_NAME)                                 \
	do {                                                                        \
		ui_container_mk_and_set_selector(                                         \
			&CONTAINER_NAME,                                                        \
			gui->up_trig,                                                           \
			gui->down_trig,                                                         \
			gui->selc_trig);                                                        \
	} while(0); 

#define MAKE_CONTAINER_BEGIN(CONTAINER_NAME)                                  \
	ui_container_t CONTAINER_NAME;                                              \
	ui_container_init(&CONTAINER_NAME)

#define MAKE_CONTAINER_END(CONTAINER_NAME, CONTAINER_ID)                      \
	CONTAINER_ID =                                                              \
		ui_container_group_add(                                                   \
			&gui->container_group,                                                  \
			&CONTAINER_NAME);

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
	u16 get_student_id_cid;

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
		"Get Student Id",
		change_container,
		&gui->get_student_id_cid
		);
	MAKE_AND_ADD_SELECTOR(con);

	MAKE_CONTAINER_END(con, gui->main_cid);
	current_container_id = gui->main_cid;
}

static void get_student_id_by_name(void* arg) {
	ui_input_t name_in;
	ui_input_init(&name_in);

	ui_input_get("Enter Name", &name_in);
}

static void get_student_id_by_age(void* arg) {
	;;
}

static void make_container_get_student_id(gui_t* gui) {
	MAKE_CONTAINER_BEGIN(con);

	ui_container_mk_and_set_header(&con, "Get Student Id");
	ui_container_mk_and_add_opt(
		&con,
		"Search By Name",
		get_student_id_by_name,
		NULL);
	ui_container_mk_and_add_opt(
		&con,
		"Search By Room",
		get_student_id_by_age,
		NULL);
	MAKE_AND_ADD_SELECTOR(con);

	MAKE_CONTAINER_END(con, gui->get_student_id_cid);
}

static void make_containers(gui_t* gui) {
	ui_container_group_init(&gui->container_group);

	make_container_main(gui);
	make_container_get_student_id(gui);
}

static void render_container(gui_t* gui, ui_container_t* con) {
	ui_renderer_clear(&gui->renderer);
	ui_render_container(&gui->renderer, con);
	ui_renderer_draw(&gui->renderer);
}

static void init(gui_t* gui) {
	make_renderer(gui);
	make_trig(gui);
	make_containers(gui);

	ui_renderer_ready();

	gui->should_update = TRUE;
	gui->running = TRUE;
}

static void deinit() {
	ui_renderer_unready();
}

static void loop(gui_t* gui) {
	if (kbio_ch == 'q') {
		gui->running = FALSE;
		return;
	}
	
	ui_container_t* current_container = 
		ui_container_group_get(
			&gui->container_group,
			current_container_id);

	if (gui->should_update) {
		render_container(gui, current_container);
	}

	// input must be check before container loop is call
	kbio_check_input_block();
	gui->should_update = ui_container_loop(current_container);

	usleep(10);
}

void gui_main() {
	gui_t gui;
	
	init(&gui);

	while (gui.running) {
		loop(&gui);
	}

	deinit();
}
