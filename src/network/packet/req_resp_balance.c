#include "req_resp_balance.h"

#include <stdlib.h>

void pkt_make_req_balance(pkt_req_balance_t* pkt, u16 id) {
	pkt_make_header(&pkt->header, REQ_BALANCE, PKT_REQ_BALANCE_PAYLOAD_SIZE);
	pkt->id = id;
}

void pkt_make_resp_balance(pkt_resp_balance_t* pkt, i32 balance) {
	pkt_make_header(&pkt->header, RESP_BALANCE, PKT_RESP_BALANCE_PAYLOAD_SIZE);
	pkt->balance = balance;
}
