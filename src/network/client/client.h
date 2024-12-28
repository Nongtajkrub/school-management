#pragma once

#include <type.h>
#include <arpa/inet.h>

typedef enum : u8 {
	BALANCE
} req_type_t;

typedef struct {
	// address infomation
	struct sockaddr_in addr;

	bool running;

	// sockets
	i32 sockfd;
} client_t;

void cli_init(client_t* cli);
void cli_deinit(client_t* cli);

void cli_ping(client_t* cli);
void cli_req_balance(client_t* cli, u16 id);
