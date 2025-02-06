#include "req_resp_id_from_name.h"

bool pkt_make_req_id_from_name(pkt_req_id_from_name_t* pkt, char* name) {
	const usize name_size = strlen(name);

	if (name_size >= MAX_STUDENT_NAME_LEN - 1) {
		return FALSE;
	}

	pkt_make_header(
			&pkt->header,
			REQ_ID_BY_NAME,
			PKT_REQ_ID_FROM_NAME_PAYLOAD_SIZE);
	memset(pkt->name, '\0', MAX_STUDENT_NAME_LEN);
	memcpy(pkt->name, name, name_size);

	return TRUE;
}

void pkt_make_resp_id_from_name(pkt_resp_id_from_name_t* pkt, i32 id) {
	pkt->id = id;
}
