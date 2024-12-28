#pragma once

#include "packet.h"

#define PKT_REQ_BALANCE_SIZE sizeof(pkt_req_balance_t)
#define PKT_REQ_BALANCE_PAYLOAD_SIZE PKT_REQ_BALANCE_SIZE - PKT_HEADER_SIZE
typedef struct {
	pkt_header_t header;

	u16 id;
} pkt_req_balance_t;

void pkt_make_req_balance(pkt_req_balance_t* pkt, u16 id);
bool pkt_send_req_balance(i32 sockfd, pkt_req_balance_t* pkt);
bool pkt_recv_req_balance(
	i32 sockfd,
	pkt_header_t* header,
	pkt_recver_t* recver 
	);

#define PKT_RESP_BALANCE_SIZE sizeof(pkt_resp_balance_t)
#define PKT_RESP_BALANCE_PAYLOAD_SIZE PKT_RESP_BALANCE_SIZE - PKT_HEADER_SIZE
typedef struct {
	pkt_header_t header;

	u16 balance;
} pkt_resp_balance_t;

void pkt_make_resp_balance(pkt_resp_balance_t* pkt, u16 balance);
bool pkt_send_resp_balance(i32 sockfd, pkt_resp_balance_t* pkt);
bool pkt_recv_resp_balance(
	i32 sockfd,
	pkt_header_t* header,
	pkt_recver_t* recver 
	);
