#include "req_resp_id_by_name.h"

bool pkt_make_req_id_by_name(pkt_req_id_by_name_t* pkt, char* name) {
	const usize name_size = strlen(name);

	if (name_size >= MAX_STUDENT_NAME_LEN - 1) {
		return FALSE;
	}

	pkt_make_header(
			&pkt->header,
			REQ_ID_BY_NAME,
			PKT_REQ_ID_BY_NAME_PAYLOAD_SIZE);
	memset(pkt->name, '\0', MAX_STUDENT_NAME_LEN);
	memcpy(pkt->name, name, name_size);

	return TRUE;
}

void pkt_make_resp_id_by_name(pkt_resp_id_by_name_t* pkt, i32 id) {
	pkt_make_header(
			&pkt->header,
			RESP_ID_BY_NAME,
			PKT_RESP_ID_BY_NAME_PAYLOAD_SIZE);

	pkt->id = id;
}
