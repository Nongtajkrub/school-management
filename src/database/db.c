#include "db.h"

#include <math.h>
#include <memory.h>

bool database_make(database_t* db, const char* dbname) {
	db->rwfd = dbio_make_rwfd(dbname);
	db->afd = dbio_make_afd(dbname);
	db->size = dbio_get_file_size_fd(db->rwfd);

	return database_is_valid(db);
}

database_t database_new(const char* dbname) {
	database_t db = {
		.rwfd = dbio_make_rwfd(dbname),
		.afd = dbio_make_afd(dbname)
	};
	db.size = dbio_get_file_size_fd(db.rwfd);

	return db;
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

// TODO: Handle some edge cases
static bool load_chunk(database_t* db, u32 chunk) {
	const u32 offset = chunk * DATABASE_BLOCK_SIZE;

	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf, offset, DATABASE_BLOCK_SIZE, CHUNK_BUF_SIZE);
}

static inline bool load_excess_blocks(database_t* db, u32 full_chunck_n) {
	const u32 offset = full_chunck_n * CHUNK_SIZE;

	memset(db->chunk_buf, 0, CHUNK_BUF_SIZE);
	return dbio_read_fd(
		db->rwfd,
		(char*)db->chunk_buf,
		offset, DATABASE_BLOCK_SIZE, get_excess_block_n(db));
}

bool database_find_id_by_name(database_t* db, const char* name, u32* buf) {
	const u32 full_chunck_n = get_full_chunck_n(db);
	const u32 all_blokc_n = get_all_block_n(db);
	const u32 excess_blokc_n = get_excess_block_n(db);

	for (u32 i = 0; i < full_chunck_n; i++) {
		printf("Chunck -> %u\n", i);
		load_chunk(db, i);

		for (u32 i = 0; i < CHUNK_BUF_SIZE; i++) {
			printf("buf -> %u, name -> %s\n", i, db->chunk_buf[i].name);
		}

		printf("\n");
	}

	load_excess_blocks(db, full_chunck_n);

	printf("Excess blocks loaded\n");
	for (u32 i = 0; i < excess_blokc_n; i++) {
		printf("buf -> %u, name -> %s\n", i, db->chunk_buf[i].name);
	}

	return true;
}
