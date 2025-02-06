#pragma once

#define GET_DATABASE_SETTINGS

#include "../../settings.h"
#include "packet.h"

#define PKT_REQ_ID_FROM_NAME_SIZE sizeof(pkt_req_id_from_name_t)
#define PKT_REQ_ID_FROM_NAME_PAYLOAD_SIZE \
	PKT_REQ_ID_FROM_NAME_SIZE - PKT_HEADER_SIZE
typedef struct {
	pkt_header_t header;

	char name[MAX_STUDENT_NAME_LEN];
} pkt_req_id_from_name_t;

// return whether the packet is created
bool pkt_make_req_id_from_name(pkt_req_id_from_name_t* pkt, char* name);

#define PKT_RESP_ID_FROM_NAME_SIZE sizeof(pkt_resp_id_from_name_t)
#define PKT_RESP_ID_FROM_NAME_PAYLOAD_SIZE \
	PKT_RESP_ID_FROM_NAME_SIZE - PKT_HEADER_SIZE
typedef struct {
	pkt_header_t header;

	i32 id;
} pkt_resp_id_from_name_t;

void pkt_make_resp_id_from_name(pkt_resp_id_from_name_t* pkt, i32 id);
