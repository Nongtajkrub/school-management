#pragma once

#include <type.h>
#include <string.h>

typedef struct {
	usize size;
	byte data[];
} packet_t;

bool packet_send(i32 sockfd, const void* data, usize size);

static inline bool packet_send_str(i32 sockfd, const char* str) {
	return packet_send(sockfd, str, strlen(str) + 1);
}

bool packet_recv(i32 sockfd, packet_t** buf);

static inline void packet_destroy(packet_t* packet) {
	free(packet);
}
