#include "client.h"
#include "../err_msg.h"

#include <type.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 8080
#define ADDR "127.0.0.1"

typedef struct {
	// address infomation
	struct sockaddr_in addr;
	usize addr_len;

	// sockets
	i32 sockfd;
} client_t;

static void init(client_t* cli, u16 port, const char* addr) {
	// create a client socket
	cli->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli->sockfd < 0) {
		perror(CREATE_SOCK_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	cli->addr.sin_family = AF_INET;
	cli->addr.sin_port = htons(port);
	
	// convert address from text to binary
	if (inet_pton(AF_INET, addr, &cli->addr.sin_addr) < 0) {
		perror(INVALID_ADDR_ERR_MSG);
		exit(EXIT_FAILURE);
	}
}

static void connect_to_serv(client_t* cli) {
	if (
		connect(
			cli->sockfd,
			(struct sockaddr*)&cli->addr,
			sizeof(cli->addr)
			) < 0
		) {
		perror(CONNECT_ERR_MSG);
		exit(EXIT_FAILURE);
	}
}

static void deinit(client_t* cli) {
	close(cli->sockfd);
}

void cli_main() {
	for (u8 i = 0; i < 15; i++) {
		client_t cli;

		init(&cli, PORT, ADDR);

		connect_to_serv(&cli);
		printf("Connected to server! Number %d\n", i);
	}
}
