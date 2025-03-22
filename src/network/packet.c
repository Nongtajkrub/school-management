#define NETIO_ENABLE_LOG

#include "packet.h"
#include "networkio.h"

#include <memory.h>

#define CALC_PACKET_SIZE(DATA_SIZE) sizeof(packet_t) + DATA_SIZE

static packet_t* packet_new(const void* data, usize size) {
	packet_t* packet = malloc(CALC_PACKET_SIZE(size));
	ASSERT(packet != NULL, DEF_ALLOC_ERRMSG);

	packet->size = size;
	memcpy(packet->data, data, size);

	return packet;
}

bool packet_send(i32 sockfd, const void* data, usize size) {
	packet_t* packet = packet_new(data, size);
	const bool r = netio_send(sockfd, packet, CALC_PACKET_SIZE(size), true);
	packet_destroy(packet);

	return r;
}

bool packet_recv(i32 sockfd, packet_t** buf) {
	// recv the packet size first
	usize size;

	if (!netio_recv(sockfd, &size, sizeof(size), true)) {
		return false;
	}

	// initilize the packet and recv the data
	*buf = malloc(CALC_PACKET_SIZE(size));
	ASSERT(buf != NULL, DEF_ALLOC_ERRMSG);

	(*buf)->size = size;

	if (!netio_recv(sockfd, (*buf)->data, size, true)) {
		free(buf);
		return false;
	}

	return true;
}
