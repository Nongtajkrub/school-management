#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "client.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../request/request.h"
#include "../networkio.h"

#include <type.h>
#include <memory.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
	if (connect(cli->sockfd,
				(struct sockaddr*)&cli->addr, sizeof(cli->addr)) < 0) 
	{
		perror(CONNECT_ERRMSG);
		exit(EXIT_FAILURE);
	}
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
	cli.running = true;	

	req_t req;

	req_make(&req, REQ_TYPE_ID_BY_NAME, "s", "Taj Borthwick");

	if (!req_send(&req, cli.sockfd)) {
		printf("did not send!\n");
	} else {
		printf("send request -> %s\n", var_string_get(&req));
	}

	sleep(1);
	cli_deinit(&cli);
}
