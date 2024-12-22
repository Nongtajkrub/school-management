#include "packet.h"
#include "networkio.h"

static inline bool send_header(i32 sockfd, pkt_header_t* header) {
	return (netio_send(sockfd, header, PKT_HEADER_SIZE, TRUE));
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
			TRUE)
		) {
		return FALSE;
	}
	return TRUE;
}

static bool recv_header(i32 sockfd, pkt_header_t* buf) {
	return netio_recv(sockfd, buf, PKT_HEADER_SIZE, TRUE);
}

static bool recv_res_id(i32 sockfd, pkt_header_t header, void* buf) {
	// set header to recv header
	res_id_pkt_t pkt = {
		.header = header
	};

	// recv the rest of the packet that is not the header
	if (
		!netio_recv(
			sockfd,
			(void*)((char*)&pkt + PKT_HEADER_SIZE),
			RES_ID_PKT_SIZE - PKT_HEADER_SIZE,
			TRUE)
		) {
		return FALSE;
	}

	memcpy(buf, &pkt, RES_ID_PKT_SIZE);
	return TRUE;
}

// return NONE mean error
pkt_type_t recv_pkt(i32 sockfd, void* buf) {
	pkt_header_t header;

	if (!recv_header(sockfd, &header)) {
		return NONE;
	}

	switch (header.type) {
	// ping and request id only have a packet header as a payload 
	case PING:
	case REQ_ID:
		memcpy(buf, &header, PKT_HEADER_SIZE);
		break;
	case RES_ID:
		if (!recv_res_id(sockfd, header, buf)) {
			return NONE;
		}
		break;
	default:
		break;
	}

	return header.type;
}
