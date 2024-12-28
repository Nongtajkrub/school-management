#include "ping.h"

void pkt_make_ping(pkt_ping_t* pkt) {
	pkt_make_header(&pkt->header, PING, PKT_PING_PAYLOAD_SIZE);
}

bool pkt_std_handle_ping(i32 sockfd) {
	pkt_ping_t respond;

	pkt_make_ping(&respond);
	return pkt_send(sockfd, &respond.header, &respond);
}
