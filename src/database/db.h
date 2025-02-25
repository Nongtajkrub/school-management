#pragma once

#include "student.h"

#include <vector.h>

#ifdef DB_ENABLE_LOG
	#define DB_LOG(MSG) perror(MSG)
#else 
	#define DB_LOG(MSG) ;;
#endif

typedef enum {
	LOAD,
	SAVE
} database_type_t;

typedef struct {
	database_type_t type;
	FILE* fd;

	usize size;
	usize data_reigion_size;
	vec_t data;

	u16 student_count;
} database_t;

void database_make(database_t* db, const char* dbname, database_type_t type);
void database_destroy(database_t* db);

bool database_save(database_t* db);
bool database_load(database_t* db);

bool database_push(database_t* db, student_t* stu);
i32 database_id_by_name(database_t* db, const char* name);
const student_t* database_student_by_id(database_t* db, u16 id);
