#pragma once

#include "packet.h"

#define PKT_PING_SIZE sizeof(pkt_ping_t)
#define PKT_PING_PAYLOAD_SIZE 0
typedef struct {
	pkt_header_t header;
} pkt_ping_t;

void pkt_make_ping(pkt_ping_t* pkt);

bool pkt_std_handle_ping(i32 sockfd);
