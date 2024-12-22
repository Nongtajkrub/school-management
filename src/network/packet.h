#pragma once

#include <type.h>
#include <string.h>

typedef enum : u8 {
	NONE,
	PING,
	REQ_ID,
	RES_ID
} pkt_type_t;

#define PKT_HEADER_SIZE sizeof(pkt_header_t)
typedef struct {
	pkt_type_t type;
	usize size;
} pkt_header_t;

#define PING_PKT_SIZE sizeof(ping_pkt_t)
typedef struct {
	pkt_header_t header;
} ping_pkt_t;

void make_ping_pkt(ping_pkt_t* pkt);
bool send_ping_pkt(i32 sockfd, ping_pkt_t* pkt);

// requesting client id and responding packet
#define REQ_ID_PKT_SIZE sizeof(req_id_pkt_t)
typedef struct {
	pkt_header_t header;
} req_id_pkt_t;

void make_req_id_pkt(req_id_pkt_t* pkt);
bool send_req_id_pkt(i32 sockfd, req_id_pkt_t* pkt);

#define RES_ID_PKT_SIZE sizeof(res_id_pkt_t)
typedef struct {
	pkt_header_t header;

	u16 id;
} res_id_pkt_t;

void make_res_id_pkt(res_id_pkt_t* pkt, u16 id);
bool send_res_id_pkt(i32 sockfd, res_id_pkt_t* pkt);

pkt_type_t recv_pkt(i32 sockfd, void* buf);
