#pragma once

#include "block.h"
#include "dbio.h"

#include <vector.h>

#ifdef DB_ENABLE_LOG
	#define DB_LOG(MSG) perror(MSG)
#else 
	#define DB_LOG(MSG) ;;
#endif

typedef struct {
	const char* name;

	// write and read file director
	FILE* rwfd;
	FILE* afd;

	// size of database
	usize size;
	
	// buffer for reading part of database
	database_block_t chunk_buf[CHUNK_BUF_SIZE];
} database_t;

bool database_make(database_t* db, const char* dbname);
void database_destroy(database_t* db);

bool database_clear(database_t* db);
bool database_append_block(database_t* db, database_block_t* block);

bool database_find_block_by_name(
	database_t* db, const char* name, database_block_t* buf);
