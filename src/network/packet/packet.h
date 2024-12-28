#pragma once

#include "../networkio.h"

#include <type.h>
#include <string.h>
#include <memory.h>

typedef enum : u8 {
	NONE,
	PING,
	REQ_BALANCE,
	RESP_BALANCE
} pkt_type_t;

#define PKT_HEADER_SIZE sizeof(pkt_header_t)
#define PKT_SKIP_HEADER(PKT) (void*)((char*)PKT + PKT_HEADER_SIZE)
typedef struct {
	pkt_type_t type;
} pkt_header_t;

#define PKT_RECVER_SIZE sizeof(pkt_recver_t)
typedef struct {
	pkt_header_t header;

	void* payload;
} pkt_recver_t;

static inline void make_header(pkt_header_t* header, pkt_type_t type) {
	header->type = type;
}

static inline bool send_header(i32 sockfd, pkt_header_t* header) {
	return netio_send(sockfd, header, PKT_HEADER_SIZE, TRUE);
}

bool pkt_recv(i32 sockfd, pkt_recver_t* recver);

void pkt_bind_payload_and_header(
	void* pkt,
	pkt_header_t* header,
	void* payload,
	usize payload_size
	);
