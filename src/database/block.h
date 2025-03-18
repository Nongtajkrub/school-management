#pragma once

#define GET_DATABASE_SETTINGS

#include "../settings.h"

#include <type.h>

// optimize for smallest size
#define DATABASE_BLOCK_SIZE sizeof(database_block_t)
typedef struct {
	// TODO: remove name len
	u8 name_len;
	char name[MAX_STUDENT_NAME_LEN];

	u8 age;
	u16 balance;
	u32 id;
} database_block_t;

typedef struct {
	// offset of the block from the beginning of the database
	// the unit is bytes
	u32 offset;
	database_block_t block;
} database_block_info_t;


bool database_block_make(
	database_block_t* data, const char* name, u32 id, u8 age, u16 balance);
