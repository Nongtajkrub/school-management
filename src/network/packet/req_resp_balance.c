#include "mk_std_send_recv_func.h"
#include "req_resp_balance.h"

#include <stdlib.h>

void pkt_make_req_balance(pkt_req_balance_t* pkt, u16 id) {
	make_header(&pkt->header, REQ_BALANCE);
	pkt->id = id;
}

MAKE_STD_PKT_SEND_AND_RECV_FUNC(
	pkt_send_req_balance,
	pkt_recv_req_balance,
	pkt_req_balance_t,
	PKT_REQ_BALANCE_PAYLOAD_SIZE
	);

void pkt_make_resp_balance(pkt_resp_balance_t* pkt, u16 balance) {
	make_header(&pkt->header, RESP_BALANCE);
	pkt->balance = balance;
}

MAKE_STD_PKT_SEND_AND_RECV_FUNC(
	pkt_send_resp_balance,
	pkt_recv_resp_balance,
	pkt_resp_balance_t,
	PKT_RESP_BALANCE_PAYLOAD_SIZE
	);
