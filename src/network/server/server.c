#define GET_SERVER_INFO
#define NETIO_LOG

#include "server.h"
#include "../err_msg.h"
#include "../settings.h"
#include "../networkio.h"
#include "../packet.h"

#include <type.h>
#include <string.h>
#include <list.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
	usize addr_len;

	// socket
	i32 sockfd;

	// clients
	list_t cli;

	struct {
		pthread_spinlock_t cli_list_lock;
	} thread_safety;
} server_t;

static void deinit(server_t* serv);

static void handle_err(server_t* serv, const char* msg, u8 do_deinit) {
	perror(msg);
	if (do_deinit) {
		deinit(serv);
	}
}

static void handle_err_and_exit(server_t* serv, const char* msg, u8 do_deinit) {
	handle_err(serv, msg, do_deinit);
	exit(EXIT_FAILURE);
}

static void init(server_t* serv, u16 port) {
	// create a server socket
	serv->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->sockfd < 0) {
		handle_err_and_exit(serv, CREATE_SOCK_ERRMSG, FALSE);
	}

	// init socket address info
	serv->addr.sin_family = AF_INET;
	serv->addr.sin_addr.s_addr = INADDR_ANY;
	serv->addr.sin_port = htons(port);
	serv->addr_len = sizeof(serv->addr);

	// bind socket to port
	if (
		bind(
			serv->sockfd,
			(struct sockaddr*)&serv->addr,
			sizeof(serv->addr)
			) < 0
		) {
		handle_err_and_exit(serv, BIND_SOCK_ERRMSG, FALSE);
	}

	LIST_MAKE(&serv->cli, client_t);
	pthread_spin_init(
		&serv->thread_safety.cli_list_lock,
		PTHREAD_PROCESS_PRIVATE
		);

	serv->running = TRUE;
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

static void disconnect_cli(server_t* serv, client_t* cli) {
	cli->connected = FALSE;
	close(cli->sockfd);
}

static void handle_pkt(server_t* serv, client_t* cli, pkt_recver_t* recver) {
	switch (recver->header.type) {
	case PING:
		if (!handle_ping_pkt(cli->sockfd)) {
			cli->connected = FALSE;
		}
		break;
	default:
		break;
	}
}

static void handle_client(server_t* serv, client_t* cli) {
	pkt_recver_t recver;

	if (!recv_pkt(cli->sockfd, &recver)) {
		cli->connected = FALSE;
		return;
	}

	handle_pkt(serv, cli, &recver);
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

	cli->thread_created = TRUE;

	while (cli->connected) {
		handle_client(serv, cli);
	}

	// disconnect client
	disconnect_cli(serv, cli);
	pthread_spin_lock(&serv->thread_safety.cli_list_lock);

	// delete client
	ssize i = list_search(&serv->cli, (void*)cli);
	if (i >= 0) {
		list_delete(&serv->cli, i);
	}

	pthread_spin_unlock(&serv->thread_safety.cli_list_lock);
	return NULL;
}

static void create_new_client(server_t* serv) {
	client_t cli;

	// make new client obj
	cli.sockfd = accept(
		serv->sockfd,
		(struct sockaddr*)&serv->addr,
		&serv->addr_len
		);
	if (cli.sockfd < 0) {
		handle_err(serv, ACCEPT_ERRMSG, TRUE);
		serv->running = FALSE;
	}

	cli.id = list_size(&serv->cli);
	cli.connected = TRUE;
	cli.thread_created = FALSE;

	// add client to connected client list
	LIST_APPEND(&serv->cli, client_t, cli);
	client_t* cli_ptr = LIST_TAIL(&serv->cli, client_t);

	// start new thread for client
	struct handle_client_thread_arg arg = {
		.serv = serv,
		.cli = cli_ptr
	};

	pthread_create(
		&cli_ptr->thread,
		NULL,
		handle_client_thread_func,
		(void*)&arg
		);
	// wait for thread to start 
	while (!cli_ptr->thread_created) {
		;;
	}
}

static void accept_and_create_client(server_t* serv) {
	// listen for connection request
	if (listen(serv->sockfd, MAX_QUEUE) < 0) {
		handle_err(serv, LISTEN_ERRMSG, TRUE);
		serv->running = FALSE;
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

	init(&serv, PORT);

	while (serv.running) {
		accept_and_create_client(&serv);
	}

	wait_for_all_thread(&serv);
	deinit(&serv);
}
