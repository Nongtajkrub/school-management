#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "server.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../networkio.h"
#include "../../database/db.h"
#include "../packet.h"

#include <list.h>
#include <type.h>
#include <string.h>
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
	database_t db;

	struct {
		pthread_spinlock_t cli_list_lock;
	} thread_safety;
} server_t;

static void deinit(server_t* serv);

static void handle_err(server_t* serv, database_t* db, const char* req) {
	perror(req);
	if (serv != NULL) {
		deinit(serv);
	}
	if (db != NULL) {
		/* TODO: Implement better database
		database_destroy(db);
		*/
	}
}

static void handle_err_and_exit(server_t* serv, database_t* db, const char* req) {
	handle_err(serv, db, req);
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

static inline void init_db(server_t* serv) {
	database_make(&serv->db, DATABASE_NAME);
}

static void deinit(server_t* serv) {
	list_it_reset(&serv->cli);

	for (u32 i = 0; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_IT_ACCESS(&serv->cli, client_t);

		close(cli->sockfd);
		list_it_increment(&serv->cli);
	}

	close(serv->sockfd);
}

static void disconnect_cli(client_t* cli) {
	cli->connected = false;
	close(cli->sockfd);
}

static void handle_client(server_t* serv, client_t* cli) {
	printf("Handling client\n");
	packet_t* buf;

	if (!packet_recv(cli->sockfd, &buf)) {
		cli->connected = false;
		printf("Error\n");
		return;
	}

	printf("recv data -> %s\n", buf->data);
}

static u32 get_cli_index_by_id(server_t* serv, u16 id) {
	list_it_reset(&serv->cli);

	for (u32 i = 0; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_IT_ACCESS(&serv->cli, client_t);

		if (cli->id == id) {
			return i;
		}

		list_it_increment(&serv->cli);
	}

	return UINT32_MAX; 
}

static void delete_client(server_t* serv, client_t* cli) {
	const u32 cli_index = get_cli_index_by_id(serv, cli->id);
	ASSERT(cli_index != UINT32_MAX, CLI_NOT_FOUND_ERRMSG);

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
	list_append_none(&serv->cli);
	client_t* cli = LIST_TAIL(&serv->cli, client_t);

	// make new client obj
	cli->sockfd = 
		accept(serv->sockfd, (struct sockaddr*)&serv->addr, &serv->addr_len);

	if (cli->sockfd < 0) {
		handle_err(serv, &serv->db, ACCEPT_ERRMSG);
		serv->running = false;
	}

	cli->id = list_size(&serv->cli);
	cli->connected = true;
	cli->thread_created = false;

	// start new thread for client
	struct handle_client_thread_arg arg = {
		.serv = serv,
		.cli = cli
	};

	pthread_create(
		&cli->thread, NULL, handle_client_thread_func, (void*)&arg);

	// wait for thread to start 
	while (!cli->thread_created) { ;; }
	printf("accept client id -> %u\n", cli->id);

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
	list_it_reset(&serv->cli);

	for (usize i = 1; i < list_size(&serv->cli); i++) {
		client_t* cli = LIST_IT_ACCESS(&serv->cli, client_t);
		pthread_join(cli->thread, NULL);

		list_it_increment(&serv->cli);
	}
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
