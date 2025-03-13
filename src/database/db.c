#include "db.h"

#include <math.h>
#include <memory.h>

static inline bool is_database_valid(database_t* db) {
	return (db->rwfd != NULL && db->afd != NULL);
}

bool database_make(database_t* db, const char* dbname) {
	db->name = dbname;
	db->rwfd = dbio_make_rwfd(dbname);
	db->afd = dbio_make_afd(dbname);
	db->size = dbio_get_file_size_fd(db->rwfd);

	return is_database_valid(db);
}

bool database_clear(database_t* db) {
	FILE* wfd = dbio_make_wfd(db->name);

	if (wfd == NULL) {
		return false;
	}
	
	dbio_close_fd(wfd);
	db->size = 0;
	return true;
}

bool database_append_block(database_t* db, database_block_t* block) {
	if (!dbio_write_fd(db->afd, (byte*)block, DATABASE_BLOCK_SIZE)) {
		return false;
	}

	db->size += DATABASE_BLOCK_SIZE;
	return true;
}

// get all blocks in databse
static inline u32 get_all_block_n(database_t* db) {
	return db->size / DATABASE_BLOCK_SIZE;
}

static inline u32 get_full_chunck_n(database_t* db) {
	return floor(((f32)get_all_block_n(db)) / ((f32)CHUNK_BUF_SIZE));
}

// get block that dont fit into chuncks
static inline u32 get_excess_block_n(database_t* db) {
	return get_all_block_n(db) - (get_full_chunck_n(db) * CHUNK_BUF_SIZE);
}

static bool load_chunk(database_t* db, u32 chunk) {
	const u32 offset = chunk * DATABASE_BLOCK_SIZE;

	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf, offset, DATABASE_BLOCK_SIZE, CHUNK_BUF_SIZE);
}

static bool load_excess_blocks(database_t* db, u32 full_chunck_n) {
	const u32 offset = full_chunck_n * CHUNK_SIZE;

	memset(db->chunk_buf, 0, CHUNK_BUF_SIZE);
	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf,
		offset, DATABASE_BLOCK_SIZE, get_excess_block_n(db));
}

bool database_find_block_by_name(
	database_t* db, const char* name, database_block_t* buf) {
	if (db->size == 0 || !is_database_valid(db)) {
		return false;
	}

	const u32 full_chunck_n = get_full_chunck_n(db);
	const u32 excess_blokc_n = get_excess_block_n(db);

	for (u32 i = 0; i < full_chunck_n; i++) {
		if (!load_chunk(db, i)) {
			return false;
		}

		for (u32 i = 0; i < CHUNK_BUF_SIZE; i++) {
			if (strcmp(db->chunk_buf[i].name, name) == 0) {
				*buf = db->chunk_buf[i];
				return true;
			}
		}
	}

	if (!load_excess_blocks(db, full_chunck_n)) {
		return false;
	}

	for (u32 i = 0; i < excess_blokc_n; i++) {

		if (strcmp(db->chunk_buf[i].name, name) == 0) {
			*buf = db->chunk_buf[i];
			return true;
		}
	}

	return false;
}
