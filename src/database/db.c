#include "db.h"

#include <math.h>
#include <memory.h>

#define BYTE_TO_CHUNK_U32(BYTE) ((u32)floor((f32)BYTE / (f32)CHUNK_SIZE))
#define BYTE_TO_BLOCK_U32(BYTE) ((u32)floor((f32)BYTE / (f32)DATABASE_BLOCK_SIZE))

#define CHUNK_TO_BLOCK(CHUNK) (CHUNK * CHUNK_BUF_SIZE)

bool database_make(database_t* db, const char* dbname) {
	db->name = dbname;
	db->rwfd = dbio_make_rwfd(dbname);
	db->size = dbio_get_file_size_fd(db->rwfd);

	return (db->rwfd != NULL);
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
	if (!dbio_write_fd(db->rwfd, (byte*)block, db->size, DATABASE_BLOCK_SIZE)) {
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
	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf,
		chunk * DATABASE_BLOCK_SIZE, DATABASE_BLOCK_SIZE, CHUNK_BUF_SIZE);
}

static bool load_excess_blocks(database_t* db, u32 full_chunck_n) {
	memset(db->chunk_buf, 0, CHUNK_BUF_SIZE);
	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf,
		full_chunck_n * CHUNK_SIZE, DATABASE_BLOCK_SIZE, get_excess_block_n(db));
}

// return whehter the operation is successful not whehter the data is found
// if the data is not found the buffer will have a size of 0
bool database_find_block_by_name(database_t* db, const char* name, vec_t* block_buf) {
	if (db->size == 0 || db->rwfd == NULL) {
		return false;
	}

	VEC_MAKE(block_buf, database_block_info_t);

	const u32 full_chunck_n = get_full_chunck_n(db);
	const u32 excess_blokc_n = get_excess_block_n(db);

	for (u32 i = 0; i < full_chunck_n; i++) {
		if (!load_chunk(db, i)) {
			return false;
		}

		for (u32 j = 0; j < CHUNK_BUF_SIZE; j++) {
			if (strcmp(db->chunk_buf[j].name, name) == 0) {
				database_block_info_t block_info = {
					.block = db->chunk_buf[j],
					.offset = ((i * CHUNK_BUF_SIZE) + j) * DATABASE_BLOCK_SIZE
				};

				vec_push(block_buf, &block_info);
			}
		}
	}

	if (!load_excess_blocks(db, full_chunck_n)) {
		return false;
	}

	for (u32 i = 0; i < excess_blokc_n; i++) {
		if (strcmp(db->chunk_buf[i].name, name) == 0) {
			database_block_info_t block_info = {
				.block = db->chunk_buf[i],
				.offset = 
					((full_chunck_n * CHUNK_BUF_SIZE) + i) * DATABASE_BLOCK_SIZE
			};

			vec_push(block_buf, &block_info);
		}
	}

	return true;
}
