#define NETIO_LOG

#include "server.h"
#include "../err_msg.h"
#include "../networkio.h"
#include "../package.h"

#include <type.h>
#include <vector.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 8080
#define MAX_QUEUE 3

typedef struct {
	bool connected;

	i32 sockfd;
} client_t;

typedef struct {
	// address infomation
	struct sockaddr_in addr;
	usize addr_len;

	// sockets
	i32 sockfd;
	vec_t cli;
} server_t;

static void deinit(server_t* serv);

static void handle_err(server_t* serv, const char* msg, u8 do_deinit) {
	perror(msg);
	if (do_deinit) {
		deinit(serv);
	}
	exit(EXIT_FAILURE);
}

static void init(server_t* serv, u16 port) {
	// create a server socket
	serv->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv->sockfd < 0) {
		handle_err(serv, CREATE_SOCK_ERRMSG, FALSE);
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
		handle_err(serv, BIND_SOCK_ERRMSG, FALSE);
	}

	VEC_MAKE(serv->cli, client_t);
}

static void deinit(server_t* serv) {
	for (u32 i = 0; i < VEC_SIZE(serv->cli); i++) {
		client_t* cli = VEC_GET(serv->cli, client_t, i);

		if (cli->connected) {
			close(cli->sockfd);
		} 
	}

	close(serv->sockfd);
}

static client_t* find_unconnected_client(server_t* serv) {
	for (u32 i = 0; i < VEC_SIZE(serv->cli); i++) {
		client_t* cli = VEC_GET(serv->cli, client_t, i);
		if (!cli->connected) {
			return cli;
		}
	}

	// no unconnected client found create a new client
	client_t cli; 
	VEC_PUSH(serv->cli, client_t, cli);
	return VEC_BACK(serv->cli, client_t);
}

// TODO: Perform a real handshake
static void perform_handshake(client_t* cli) {
	netio_send(cli->sockfd, HAND_SHAKE_MSG, HAND_SHAKE_MSG_SIZE, TRUE);
}

static void accept_connection(server_t* serv) {
	// listen for connection request
	if (listen(serv->sockfd, MAX_QUEUE) < 0) {
		handle_err(serv, LISTEN_ERRMSG, TRUE);
	}

	// accept incoming connection
	client_t* cli = find_unconnected_client(serv);

	cli->sockfd = accept(
		serv->sockfd,
		(struct sockaddr*)&serv->addr,
		&serv->addr_len
		);
	if (cli->sockfd < 0) {
		handle_err(serv, ACCEPT_ERRMSG, TRUE);
	}
	cli->connected = TRUE;

	perform_handshake(cli);
}

void serv_main() {
	server_t serv;

	init(&serv, PORT);

	while (TRUE) {
		accept_connection(&serv);
	}

	deinit(&serv);
}
