#define GET_SERVER_SETTINGS
#define NETIO_ENABLE_LOG

#include "client.h"
#include "../err_msg.h"
#include "../../settings.h"
#include "../message/request/request.h"
#include "../message/request/req_type.h"
#include "../networkio.h"
#include "../../gui/gui.h"

#include <type.h>
#include <memory.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct client {
	// address infomation
	struct sockaddr_in addr;

	bool running;

	// sockets
	i32 sockfd;
} static cli;

typedef struct client client_t;

static void init(u16 port, const char* addr) {
	// create a client socket
	cli.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli.sockfd < 0) {
		perror(CREATE_SOCK_ERRMSG);
		exit(EXIT_FAILURE);
	}

	cli.addr.sin_family = AF_INET;
	cli.addr.sin_port = htons(port);
	
	// convert address from text to binary
	if (inet_pton(AF_INET, addr, &cli.addr.sin_addr) < 0) {
		perror(INVALID_ADDR_ERRMSG);
		exit(EXIT_FAILURE);
	}
}

static void connect_to_serv() {
	if (connect(cli.sockfd,
				(struct sockaddr*)&cli.addr, sizeof(cli.addr)) < 0) 
	{
		perror(CONNECT_ERRMSG);
		exit(EXIT_FAILURE);
	}
}

static void cli_init() {
	init(PORT, ADDR);
	connect_to_serv();
}

static void cli_deinit() {
	close(cli.sockfd);
}

bool cli_req_id_by_name(const char* name, i32* buf) {
	msg_req_t req;

	req_make(
		&req,
		REQT_ID_BY_NAME, REQT_ID_BY_NAME_DATA_FMT, "Taj Borthwick");

	if (!msg_send(&req, cli.sockfd)) {
		return false;
	}

	msg_destroy(&req);

	msg_req_t rep;

	if (!msg_recv(&rep, cli.sockfd)) {
		msg_destroy(&rep);
		return false;
	}

	char* data = msg_get_data(&rep);
	*buf = atoi(data);
	msg_get_data_destroy(data);

	msg_destroy(&rep);
	return true;
}

void cli_main() {
	client_t cli;

	cli_init();
	cli.running = true;	

	gui_init();

	while (gui_should_run() && cli.running) {
		gui_loop();
	}

	cli_deinit();
	gui_deinit();
}
