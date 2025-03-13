#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "server.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../networkio.h"
#include "../message/request/request.h"
#include "../message/request/lexer.h"
#include "../message/request/handler.h"
/* TODO: Implement better database
#include "../../database/db.h"
*/

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
	vec_t cli;

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

	VEC_MAKE(&serv->cli, client_t);
	pthread_spin_init(
		&serv->thread_safety.cli_list_lock, PTHREAD_PROCESS_PRIVATE);
}

static void init_db(server_t* serv) {
	/* TODO: Implement better databsae
	database_make(&serv->db, DATABASE_NAME, LOAD);

	if (!database_load(&serv->db)) {
		handle_err_and_exit(serv, NULL, DB_LOAD_ERRMSG);
	}
	*/
}

static void deinit(server_t* serv) {
	for (u32 i = 0; i < vec_size(&serv->cli); i++) {
		client_t* cli = VEC_GET(&serv->cli, client_t, i);
		close(cli->sockfd);
	}

	close(serv->sockfd);
}

static void disconnect_cli(client_t* cli) {
	cli->connected = false;
	close(cli->sockfd);
}

static void handle_client(server_t* serv, client_t* cli) { 
	msg_req_t req;

	if (!msg_recv(&req, cli->sockfd)) {
		cli->connected = false;
		return;
	}

	//printf("recv req -> %s\n", msg_get(&req));

	if (!req_handle(&req, cli->sockfd, &serv->db)) {
		printf("Invalid req\n");
		msg_send_err(cli->sockfd);
	}

	msg_destroy(&req);
}

static u32 get_cli_index_by_id(server_t* serv, u16 id) {
	for (u32 i = 0; i < vec_size(&serv->cli); i++) {
		client_t* cli = VEC_GET(&serv->cli, client_t, i);

		if (cli->id == id) {
			return i;
		}
	}

	return UINT32_MAX; 
}

static void delete_client(server_t* serv, client_t* cli) {
	u32 cli_index = get_cli_index_by_id(serv, cli->id);
	
	if (cli_index == UINT32_MAX) {
		handle_err(serv, NULL, CLI_NOT_FOUND_ERRMSG);
		serv->running = false;
	}

	vec_swapback(&serv->cli, cli_index);
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

	cli.id = vec_size(&serv->cli);
	cli.connected = true;
	cli.thread_created = false;

	// add client to connected client list
	vec_push(&serv->cli, &cli);
	printf("accept client\n");
	client_t* cli_ptr = VEC_BACK(&serv->cli, client_t);

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
	for (usize i = 1; i < vec_size(&serv->cli); i++) {
		client_t* cli = VEC_GET(&serv->cli, client_t, i);
		pthread_join(cli->thread, NULL);
	}
}

#include "../../database/db.h"

void serv_main() {
	database_t db;

	if (!database_make(&db, DATABASE_NAME)) {
		perror("Fail to open databse");
		return;
	}

	if (!database_clear(&db)) {
		perror("Fail to clear datbaes");
		return;
	}

	database_block_t taj_block;

	if (!database_block_make(&taj_block, "Taj Borthwick", 16335, 15, 1000)) {
		perror("Fail to make Taj block");
		return;
	}

	database_block_t korn_block;

	if (!database_block_make(&korn_block, "Sin Ountanon", 16332, 15, 1000)) {
		perror("Fail to make Korn block");
		return;
	}

	if (!database_append_block(&db, &taj_block)) {
		perror("Fail to append Taj block");
		return;
	}

	if (!database_append_block(&db, &korn_block)) {
		perror("Fail to append Korn block");
		return;
	}

	if (!database_append_block(&db, &korn_block)) {
		perror("Fail to append Korn block");
		return;
	}

	vec_t blocks;

	if (!database_find_block_by_name(&db, "Sin Ountanon", &blocks)) {
		perror("Fail to find Korn id");
		return;
	}

	if (vec_size(&blocks) == 0) {
		perror("Invalid name");
	}

	for (u32 i = 0; i < vec_size(&blocks); i++) {
		printf(
			"Sin Ountanon offset -> %d\n",
			(VEC_GET(&blocks, database_block_info_t, i))->offset);
	}

	vec_destroy(&blocks);

	if (!database_find_block_by_name(&db, "Taj Borthwick", &blocks)) {
		perror("Fail to find Taj id");
	}

	if (vec_size(&blocks) == 0) {
		perror("Invalid name");
	}

	for (u32 i = 0; i < vec_size(&blocks); i++) {
		printf(
			"Taj Borthwick offset -> %d\n",
			(VEC_GET(&blocks, database_block_info_t, i))->offset);
	}

	/*
	server_t serv;

	init_serv(&serv, PORT);
	init_db(&serv);

	serv.running = true;
	while (serv.running) {
		accept_and_create_client(&serv);
	}

	wait_for_all_thread(&serv);
	deinit(&serv);
	*/
}
