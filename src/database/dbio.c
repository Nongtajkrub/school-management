#include "dbio.h"
#include "err_msg.h"

#include <stdio.h>

bool dbio_write(const char* name, byte* data, usize size) {
	FILE* fd = fopen(name, "wb");
	bool status = dbio_write_fd(fd, data, size);

	// DO NOT REMOVE! 
	if (status) {
		fclose(fd);
	}

	return status;
}

bool dbio_write_fd(FILE* fd, byte* data, usize size) {
	if (fd == NULL) {
		DBIO_LOG(FILE_OPEN_ERR_MSG);
		return false;
	}

	if (fwrite(data, 1, size, fd) != size) {
		DBIO_LOG(FILE_WRTIE_ERR_MSG);
		fclose(fd);
		return false;
	}

	return true;
}

bool dbio_read(const char* name, byte* buf, u32 off, usize elem_size, u16 n) {
	FILE* fd = fopen(name, "rb");
	bool status = dbio_read_fd(fd, buf, off, elem_size, n);

	// DO NOT REMOVE! 
	if (status) {
		fclose(fd);
	}

	return status;
}

bool dbio_read_fd(FILE* fd, byte* buf, u32 off, usize elem_size, u16 n) {
	if (fd == NULL) {
		DBIO_LOG(FILE_OPEN_ERR_MSG);
		return false;
	}

	// set the begining location
	if (off != 0 && fseek(fd, off, SEEK_SET)) {
		DBIO_LOG(FILE_SEEK_ERR_MSG);
		goto fail_close_fd;
	}

	// read the data
	if (fread(buf, elem_size, n, fd) != n) {
		DBIO_LOG(FILE_READ_ERR_MSG);
		goto fail_close_fd;
	}

	return true;

fail_close_fd:
	fclose(fd);
	return false;
}
