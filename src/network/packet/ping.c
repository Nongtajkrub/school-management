#include "ping.h"

void pkt_make_ping(pkt_ping_t* pkt) {
	make_header(&pkt->header, PING);
}

bool pkt_send_ping(i32 sockfd, pkt_ping_t* pkt) {
	return send_header(sockfd, &pkt->header);
}

bool pkt_std_handle_ping(i32 sockfd) {
	pkt_ping_t respond;

	pkt_make_ping(&respond);
	return pkt_send_ping(sockfd, &respond);
}
