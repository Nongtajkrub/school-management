#include "packet.h"

#include "req_resp_balance.h"
#include "ping.h"

#include <memory.h>
#include <stdlib.h>

static inline bool recv_header(i32 sockfd, pkt_header_t* buf) {
	return netio_recv(sockfd, buf, PKT_HEADER_SIZE, TRUE);
}

bool pkt_recv(i32 sockfd, pkt_recver_t* recver) {
	pkt_header_t header;

	if (!recv_header(sockfd, &header)) {
		return FALSE;
	}

	switch (header.type) {
	// ping and request id only have a packet header as a payload 
	case PING:
		recver->header = header;
		break;
	case REQ_BALANCE:
		return pkt_recv_req_balance(sockfd, &header, recver);
		break;
	case RESP_BALANCE:
		return pkt_recv_resp_balance(sockfd, &header, recver);
		break;
	default:
		break;
	}

	return TRUE;
}

void pkt_bind_payload_and_header(
	void* pkt,
	pkt_header_t* header,
	void* payload,
	usize payload_size
	) {
	memcpy(pkt, header, PKT_HEADER_SIZE);
	memcpy((void*)((char*)pkt + PKT_HEADER_SIZE), payload, payload_size);
}
