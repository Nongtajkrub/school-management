#include "packet.h"
#include "networkio.h"

#include <memory.h>
#include <stdlib.h>

static inline bool send_header(i32 sockfd, pkt_header_t* header) {
	return netio_send(sockfd, header, PKT_HEADER_SIZE, TRUE);
}

static inline bool recv_header(i32 sockfd, pkt_header_t* buf) {
	return netio_recv(sockfd, buf, PKT_HEADER_SIZE, TRUE);
}

static void make_header(pkt_header_t* header, pkt_type_t type, usize size) {
	header->type = type;
	header->size = size;
}

void make_ping_pkt(ping_pkt_t* pkt) {
	make_header(&pkt->header, PING, PING_PKT_SIZE);
}

bool send_ping_pkt(i32 sockfd, ping_pkt_t* pkt) {
	return send_header(sockfd, &pkt->header);
}

void make_req_id_pkt(req_id_pkt_t* pkt) {
	make_header(&pkt->header, REQ_ID, REQ_ID_PKT_SIZE);
}

bool send_req_id_pkt(i32 sockfd, req_id_pkt_t* pkt) {
	return send_header(sockfd, &pkt->header);
}

void make_res_id_pkt(res_id_pkt_t* pkt, u16 id) {
	make_header(&pkt->header, RES_ID, RES_ID_PKT_SIZE);
	pkt->id = id;
}

bool send_res_id_pkt(i32 sockfd, res_id_pkt_t* pkt) {
	// send the header
	if (!send_header(sockfd, &pkt->header)) {
		return FALSE;
	}
	// send the payload without the header
	if (
		!netio_send(
			sockfd,
			(void*)(char*)pkt + PKT_HEADER_SIZE,
			RES_ID_PKT_SIZE - PKT_HEADER_SIZE,
			TRUE
			)
		) {
		return FALSE;
	}
	return TRUE;
}

static bool recv_res_id(i32 sockfd, pkt_header_t header, pkt_recver_t* recver) {
	usize payload_size = RES_ID_PKT_SIZE - PKT_HEADER_SIZE;

	// set header to recv header
	recver->header = header;

	// recv the rest of the packet that is not the header (recv payload)
	recver->payload = malloc(payload_size);
	return netio_recv(sockfd, recver->payload, payload_size, TRUE);
}

bool recv_pkt(i32 sockfd, pkt_recver_t* recver) {
	pkt_header_t header;

	if (!recv_header(sockfd, &header)) {
		return FALSE;
	}

	switch (header.type) {
	// ping and request id only have a packet header as a payload 
	case PING:
	case REQ_ID:
		recver->header = header;
		break;
	case RES_ID:
		if (!recv_res_id(sockfd, header, recver)) {
			return FALSE;
		}
		break;
	default:
		break;
	}

	return TRUE;
}

// return true if successfull else return false
bool handle_ping_pkt(i32 sockfd) {
	ping_pkt_t ping_reply;
	make_ping_pkt(&ping_reply);

	return send_ping_pkt(sockfd, &ping_reply);
}
