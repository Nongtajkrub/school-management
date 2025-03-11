#pragma once

#include <type.h>
#include <stdio.h>

#ifdef DBIO_ENABLE_LOG
	#define DBIO_LOG(MSG) perror(MSG)
#else 
	#define DBIO_LOG(MSG) ;;
#endif

static inline FILE* dbio_make_wfd(const char* file) {
	return fopen(file, "wb");
}

static inline FILE* dbio_make_afd(const char* file) {
	return fopen(file, "ab");
}

static inline FILE* dbio_make_rfd(const char* file) {
	return fopen(file, "rb");
}

static inline FILE* dbio_make_rwfd(const char* file) {
	return fopen(file, "r+");
}

static inline void dbio_close_fd(FILE* fd) {
	fclose(fd);
}

// dbio function that take in a fd will only close the fd if an error occure

// return whether operation is succesfull
bool dbio_write(const char* name, byte* data, usize size);
bool dbio_write_fd(FILE* fd, byte* data, usize size);

// return whether operation is succesfull
bool dbio_read(const char* name, byte* buf, u32 off, usize elem_size, u16 n);
bool dbio_read_fd(FILE* fd, byte* buf, u32 off, usize elem_size, u16 n);

usize dbio_get_file_size(const char* name);
usize dbio_get_file_size_fd(FILE* fd);
