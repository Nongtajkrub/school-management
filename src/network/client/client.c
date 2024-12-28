#define GET_SERVER_INFO
#define NETIO_LOG

#include "client.h"
#include "../err_msg.h"
#include "../settings.h"
#include "../networkio.h"
#include "../packet/packet_all.h"

#include <type.h>
#include <queue.h>
#include <memory.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define REQ_STACK_CAPACITY 12

typedef struct {
	req_type_t type;

	u16 id;
} req_t;

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

void cli_ping(client_t* cli) {
	pkt_ping_t ping;

	pkt_make_ping(&ping);
	if (!pkt_send(cli->sockfd, &ping.header, NULL)) {
		exit(EXIT_FAILURE);
	}
	printf("Ping sent\n");
	
	pkt_recver_t respond;

	if (!pkt_recv(cli->sockfd, &respond)) {
		exit(EXIT_FAILURE);
	}

	if (respond.header.type == PING) {
		printf("Recv respond!\n");
	} else {
		printf("Did not recv respond!\n");
	}
}

void cli_req_balance(client_t* cli, u16 id) {
	pkt_req_balance_t req_pkt;

	pkt_make_req_balance(&req_pkt, id);
	if (!pkt_send(cli->sockfd, &req_pkt.header, &req_pkt)) {
		exit(EXIT_FAILURE);
	}

	pkt_recver_t recver;

	if (!pkt_recv(cli->sockfd, &recver)) {
		exit(EXIT_FAILURE);
	}

	if (recver.header.type != RESP_BALANCE) {
		printf("Recv wrong respond! -> %d\n", recver.header.type);
		exit(EXIT_FAILURE);
	}

	pkt_resp_balance_t resp_pkt;

	pkt_bind_payload_and_header(
		&resp_pkt,
		&recver.header,
		recver.payload,
		PKT_RESP_BALANCE_PAYLOAD_SIZE
		);

	printf("Balance -> %d\n", resp_pkt.balance);
}

void cli_init(client_t* cli) {
	init(cli, PORT, ADDR);
	connect_to_serv(cli);
}

void cli_deinit(client_t* cli) {
	close(cli->sockfd);
}
