#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "server.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../networkio.h"
#include "../request/request.h"
#include "../../database/db.h"

#include <type.h>
#include <string.h>
#include <list.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DATABASE_NAME "database.db"

#define MAX_QUEUE 3

typedef struct {
	bool connected;

	i32 sockfd;
	u16 id;

	bool thread_created;
	pthread_t thread;
} client_t;

typedef struct {
	bool running;

	// address infomation
	struct sockaddr_in addr;
	socklen_t addr_len;

	// socket
	i32 sockfd;

	// clients
	list_t cli;

	// database
	dbdata_t db;

	struct {
		pthread_spinlock_t cli_list_lock;
	} thread_safety;
} server_t;

static void deinit(server_t* serv);

static void handle_err(server_t* serv, dbdata_t* db, const char* msg) {
	perror(msg);
	if (serv != NULL) {
		deinit(serv);
	}
	if (db != NULL) {
		dbdata_destroy(db);
	}
}

static void handle_err_and_exit(server_t* serv, dbdata_t* db, const char* msg) {
	handle_err(serv, db, msg);
	exit(EXIT_FAILURE);
}

static void init_serv(server_t* serv, u16 port) {
	// create a server socket
	serv->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->sockfd < 0) {
		handle_err_and_exit(NULL, NULL, CREATE_SOCK_ERRMSG);
	}

	// init socket address info
	serv->addr.sin_family = AF_INET;
	serv->addr.sin_addr.s_addr = INADDR_ANY;
	serv->addr.sin_port = htons(port);
	serv->addr_len = sizeof(serv->addr);

	// bind socket to port
	if (bind(
			serv->sockfd,
		   	(struct sockaddr*)&serv->addr, sizeof(serv->addr)) < 0) {
		handle_err_and_exit(NULL, NULL, BIND_SOCK_ERRMSG);
	}

	LIST_MAKE(&serv->cli, client_t);
	pthread_spin_init(
		&serv->thread_safety.cli_list_lock, PTHREAD_PROCESS_PRIVATE);
}

static void init_db(server_t* serv) {
	dbdata_make(&serv->db, DATABASE_NAME, LOAD);

	if (!dbdata_load(&serv->db)) {
		handle_err_and_exit(serv, NULL, DB_LOAD_ERRMSG);
	}
}

static void deinit(server_t* serv) {
	list_reset_it(&serv->cli);

	for (u32 i = 0; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_ACCESS_IT(&serv->cli, client_t);

		close(cli->sockfd);
		list_increment_it(&serv->cli);
	}

	list_reset_it(&serv->cli);
	list_delete_all(&serv->cli);
	close(serv->sockfd);
}

static void disconnect_cli(client_t* cli) {
	cli->connected = false;
	close(cli->sockfd);
}

static void handle_client(server_t* serv, client_t* cli) { 
	req_t req;

	if (!req_recv(&req, cli->sockfd)) {
		req_destroy(&req);
		cli->connected = false;
		return;
	}

	printf("recv req -> %s\n", req_get(&req));
	req_destroy(&req);
}

static u32 get_cli_index_by_id(server_t* serv, u16 id) {
	list_reset_it(&serv->cli);

	for (u32 i = 0; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_ACCESS_IT(&serv->cli, client_t);

		if (cli->id == id) {
			return i;
		}

		list_increment_it(&serv->cli);
	}

	return UINT32_MAX; 
}

static void delete_client(server_t* serv, client_t* cli) {
	u32 cli_index = get_cli_index_by_id(serv, cli->id);
	
	if (cli_index == UINT32_MAX) {
		handle_err(serv, NULL, CLI_NOT_FOUND_ERRMSG);
		serv->running = false;
	}

	list_delete(&serv->cli, cli_index);
}

struct handle_client_thread_arg {
	server_t* serv;
	client_t* cli;
};

static void* handle_client_thread_func(void* _arg) {
	struct handle_client_thread_arg* arg = 
		(struct handle_client_thread_arg*)_arg;
	server_t* serv = arg->serv;
	client_t* cli = arg->cli;

	cli->thread_created = true;

	while (cli->connected) {
		handle_client(serv, cli);
	}

	// disconnect client
	disconnect_cli(cli);
	pthread_spin_lock(&serv->thread_safety.cli_list_lock);

	delete_client(serv, cli);

	pthread_spin_unlock(&serv->thread_safety.cli_list_lock);
	return NULL;
}

static void create_new_client(server_t* serv) {
	client_t cli;

	// make new client obj
	cli.sockfd = 
		accept(serv->sockfd, (struct sockaddr*)&serv->addr, &serv->addr_len);

	if (cli.sockfd < 0) {
		handle_err(serv, &serv->db, ACCEPT_ERRMSG);
		serv->running = false;
	}

	cli.id = list_size(&serv->cli);
	cli.connected = true;
	cli.thread_created = false;

	// add client to connected client list
	list_append(&serv->cli, &cli);
	printf("accept client\n");
	client_t* cli_ptr = LIST_TAIL(&serv->cli, client_t);

	// start new thread for client
	struct handle_client_thread_arg arg = {
		.serv = serv,
		.cli = cli_ptr
	};

	pthread_create(
		&cli_ptr->thread, NULL, handle_client_thread_func, (void*)&arg);

	// wait for thread to start 
	while (!cli_ptr->thread_created) { ;; }
}

static void accept_and_create_client(server_t* serv) {
	// listen for connection request
	if (listen(serv->sockfd, MAX_QUEUE) < 0) {
		handle_err(serv, &serv->db, LISTEN_ERRMSG);
		serv->running = false;
	}

	// accept incoming connection
	create_new_client(serv);
}

static void wait_for_all_thread(server_t* serv) {
	list_reset_it(&serv->cli);

	for (usize i = 1; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_ACCESS_IT(&serv->cli, client_t);

		pthread_join(cli->thread, NULL);
		list_increment_it(&serv->cli);
	}

	list_reset_it(&serv->cli);
}

void serv_main() {
	server_t serv;

	init_serv(&serv, PORT);
	init_db(&serv);

	serv.running = true;
	while (serv.running) {
		accept_and_create_client(&serv);
	}

	wait_for_all_thread(&serv);
	deinit(&serv);
}
