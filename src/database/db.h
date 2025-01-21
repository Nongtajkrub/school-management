#pragma once

#include "student.h"

#include <vector.h>

typedef enum {
	LOAD,
	SAVE
} dbdata_type_t;

typedef struct {
	dbdata_type_t type;
	FILE* fd;

	usize size;
	vec_t data;
} dbdata_t;

void dbdata_make(dbdata_t* db, const char* dbname, dbdata_type_t type);

bool dbdata_push(dbdata_t* db, student_t* stu);

bool dbdata_save(dbdata_t* db);
bool dbdata_load(dbdata_t* db);
