#include "networkio.h"
#include "err_msg.h"

#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>

static bool send_block(i32 sockfd, const void* buf, usize size) {
	if (send(sockfd, buf, size, 0) < 0) {
		NETIO_LOG(SEND_ERRMSG);
		return FALSE;
	}
	return TRUE;
}

static bool send_none_block(i32 sockfd, const void* buf, usize size) {
	usize total_sent = 0;

	while (total_sent != size) {
		ssize sent = send(
			sockfd,
			(const char*)buf + total_sent,
			size - total_sent,
			MSG_DONTWAIT
			);

		if (sent < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				continue;
			}
			NETIO_LOG(SEND_ERRMSG);
			return FALSE;
		}

		total_sent += (usize)sent;
	}

	return TRUE;
}

// return true if successfull else return false
bool netio_send(i32 sockfd, const void* buf, usize size, bool block) {
	return (block) ?
		send_block(sockfd, buf, size) :
		send_none_block(sockfd, buf, size);
}

static bool recv_block(i32 sockfd, void *buf, usize size) {
	// recving zero bytes is also an error
	if (recv(sockfd, buf, size, 0) <= 0) {
		NETIO_LOG(RECV_ERRMSG);
		return FALSE;
	}
	return TRUE;
}

static bool recv_none_block(i32 sockfd, void *buf, usize size) {
	NETIO_LOG(DEF_NOT_IMPLEMENTED_ERRMSG);
	return FALSE;
}

bool netio_recv(i32 sockfd, void *buf, usize size, bool block) {
	return (block) ?
		recv_block(sockfd, buf, size) :
		recv_none_block(sockfd, buf, size);
}
