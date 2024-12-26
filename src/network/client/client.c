#define GET_SERVER_INFO
#define NETIO_LOG

#include "client.h"
#include "../err_msg.h"
#include "../settings.h"
#include "../networkio.h"
#include "../packet.h"

#include <type.h>
#include <memory.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define REQ_STACK_CAPACITY 12

typedef struct {
	// address infomation
	struct sockaddr_in addr;

	// sockets
	i32 sockfd;
} client_t;

static void init(client_t* cli, u16 port, const char* addr) {
	// create a client socket
	cli->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli->sockfd < 0) {
		perror(CREATE_SOCK_ERRMSG);
		exit(EXIT_FAILURE);
	}

	cli->addr.sin_family = AF_INET;
	cli->addr.sin_port = htons(port);
	
	// convert address from text to binary
	if (inet_pton(AF_INET, addr, &cli->addr.sin_addr) < 0) {
		perror(INVALID_ADDR_ERRMSG);
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
		perror(CONNECT_ERRMSG);
		exit(EXIT_FAILURE);
	}
}

static void deinit(client_t* cli) {
	close(cli->sockfd);
}

void cli_main() {
	client_t cli[10];

	for (u8 i = 0; i < 10; i++) {
		init(&cli[i], PORT, ADDR);
		connect_to_serv(&cli[i]);
		printf("Connected to server\n");
	}

	sleep(5);
	/*
	ping_pkt_t ping;
	make_ping_pkt(&ping);
	send_ping_pkt(cli.sockfd, &ping);

	pkt_recver_t recver;
	recv_pkt(cli.sockfd, &recver);

	ping_pkt_t reply;
	if (recver.header.type != PING) {
		printf("Wrong reply!\n");
	} else {
		printf("Got reply!\n");
	}
	*/

	close(cli[2].sockfd);
	printf("Client 2 exit!\n");
	sleep(5);
}
