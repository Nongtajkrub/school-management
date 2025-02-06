#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "client.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../networkio.h"
#include "../packet/packet_all.h"

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

	bool running;

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

// return whether a respond is recv
static bool ping(client_t* cli) {
	pkt_ping_t ping;
	pkt_recver_t respond;

	// send ping
	pkt_make_ping(&ping);
	if (!pkt_send(cli->sockfd, &ping.header, NULL)) {
		exit(EXIT_FAILURE);
	}

	// recv ping
	if (!pkt_recv(cli->sockfd, &respond)) {
		exit(EXIT_FAILURE);
	}

	return respond.header.type == PING;
}

// return whether request is successful however if fail to send of recv pkt
// client will disconnect
static bool req_balance(client_t* cli, u16 id, u16* buf) {
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
		return FALSE;
	}

	pkt_resp_balance_t resp_pkt;

	pkt_bind_payload_and_header(
		&resp_pkt,
		&recver.header,
		recver.payload,
		PKT_RESP_BALANCE_PAYLOAD_SIZE
		);

	*buf = resp_pkt.balance;
	return TRUE;
}

static void cli_init(client_t* cli) {
	init(cli, PORT, ADDR);
	connect_to_serv(cli);
}

static void cli_deinit(client_t* cli) {
	close(cli->sockfd);
}

void cli_main() {
	client_t cli;

	cli_init(&cli);
	cli.running = TRUE;
	
	u16 balance;

	if (!req_balance(&cli, 16335, &balance)) {
		printf("Request fail\n");
	}
	printf("balance -> %d\n", balance);

	cli_deinit(&cli);
}
