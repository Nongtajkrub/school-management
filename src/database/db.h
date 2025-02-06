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
} dbdata_type_t;

typedef struct {
	dbdata_type_t type;
	FILE* fd;

	usize size;
	usize data_reigion_size;
	vec_t data;

	u16 student_count;
} dbdata_t;

void dbdata_make(dbdata_t* db, const char* dbname, dbdata_type_t type);
void dbdata_destroy(dbdata_t* db);

bool dbdata_save(dbdata_t* db);
bool dbdata_load(dbdata_t* db);

bool dbdata_push(dbdata_t* db, student_t* stu);
i32 dbdata_id_from_name(dbdata_t* db, const char* name);
const student_t* dbdata_student_from_id(dbdata_t* db, u16 id);
