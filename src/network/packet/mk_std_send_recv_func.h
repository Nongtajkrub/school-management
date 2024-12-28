/*

#pragma once

// generates a standard function for sending packets
#define MAKE_STD_PKT_SEND_FUNC(NAME, PKT_TYPE, PAYLOAD_SIZE)                  \
	bool NAME(i32 sockfd, PKT_TYPE* pkt) {                                    \
		if (!send_header(sockfd, &pkt->header)) {                             \
			return FALSE;                                                     \
		}                                                                     \
                                                                              \
		return netio_send(                                                    \
			sockfd,                                                           \
			PKT_SKIP_HEADER(pkt),                                             \
			PAYLOAD_SIZE,                                                     \
			TRUE                                                              \
			);                                                                \
	}

// generates a standard function for recving packets
#define MAKE_STD_PKT_RECV_FUNC(NAME, PAYLOAD_SIZE)                            \
	bool NAME(i32 sockfd, pkt_header_t* header, pkt_recver_t* recver) {       \
		recver->header = *header;                                             \
		                                                                      \
		recver->payload = malloc(PAYLOAD_SIZE);                               \
		ASSERT(recver->payload != NULL, DEF_ALLOC_ERRMSG);                    \
		                                                                      \
		return netio_recv(                                                    \
			sockfd,                                                           \
			recver->payload,                                                  \
			PAYLOAD_SIZE,                                                     \
			TRUE                                                              \
			);                                                                \
	}

// generates a standard function for both sending and recving packets
#define MAKE_STD_PKT_SEND_AND_RECV_FUNC(                                      \
	SEND_NAME,                                                                \
	RECV_NAME,                                                                \
	PKT_TYPE,                                                                 \
	PAYLOAD_SIZE                                                              \
	)                                                                         \
	MAKE_STD_PKT_SEND_FUNC(SEND_NAME, PKT_TYPE, PAYLOAD_SIZE)                 \
	MAKE_STD_PKT_RECV_FUNC(RECV_NAME, PAYLOAD_SIZE)

*/
