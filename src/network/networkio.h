#pragma once

#include <type.h>

#ifdef NETIO_ENABLE_LOG
	#define NETIO_LOG(MSG) perror(MSG)
#else
	#define NETIO_LOG(MSG) ;;
#endif //#ifdef NETIO_LOG

bool netio_send(i32 sockfd, const void* buf, usize size, bool block);
bool netio_recv(i32 sockfd, void *buf, usize size, bool block);
