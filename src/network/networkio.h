#pragma once

#include <type.h>

bool netio_send(i32 sockfd, const void* buf, usize size, bool block);
bool netio_recv(i32 sockfd, void *buf, usize size, bool block);
