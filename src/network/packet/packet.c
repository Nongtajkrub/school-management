#include "packet.h"

#include "req_resp_balance.h"
#include "ping.h"

#include <memory.h>
#include <stdlib.h>

static inline bool contain_payload(pkt_header_t* header) {
	return header->payload_size != 0;
}

bool pkt_send(i32 sockfd, pkt_header_t* header, void* pkt) {
	// send header
	if (!pkt_send_header(sockfd, header)) {
		return FALSE;
	}

	if (!contain_payload(header)) {
		return TRUE;
	}

	// send payload
	return netio_send(sockfd, PKT_GET_PAYLOAD(pkt), header->payload_size, TRUE);
}

bool pkt_recv(i32 sockfd, pkt_recver_t* recver) {
	pkt_header_t header;

	// recv header
	if (!pkt_recv_header(sockfd, &header)) {
		return FALSE;
	}
	
	recver->header = header;
	if (!contain_payload(&header)) {
		return TRUE;
	}

	// alocate memory and recv payload
	recver->payload = malloc(header.payload_size);
	ASSERT(recver->payload != NULL, DEF_ALLOC_ERRMSG);

	return netio_recv(sockfd, recver->payload, header.payload_size, TRUE);
}

void pkt_bind_payload_and_header(void* pkt,
		pkt_header_t* header, void* payload, usize payload_size)
{
	memcpy(pkt, header, PKT_HEADER_SIZE);
	memcpy((void*)((char*)pkt + PKT_HEADER_SIZE), payload, payload_size);
}
