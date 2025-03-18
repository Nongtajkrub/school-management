#include "block.h"

#include <stdlib.h>
#include <string.h>

bool database_block_make(
	database_block_t* block, const char* name, u32 id, u8 age, u16 balance) {
	const usize name_len = strlen(name);
	
	if (name_len >= MAX_STUDENT_NAME_LEN - 1) {
		return false;
	}

	memset(block->name, '\0', MAX_STUDENT_NAME_LEN);
	memcpy(block->name, name, name_len);

	block->id = id;
	block->age = age;
	block->balance = balance;

	return true;
}
