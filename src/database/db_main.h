#pragma once

#include "student.h"

#define DBIO_ENABLE_LOG
#include "dbio.h"

#include <vector.h>

#ifndef DB_NAME
	#define DB_NAME "dbdata.db"
#endif // #ifndef DB_NAME

typedef vec_t dbdata_t;

static inline void dbdata_make(dbdata_t* db) {
	VEC_MAKE(db, student_t);
}

static inline void dbdata_push(dbdata_t* db, student_t* stu) {
	vec_push(db, stu);
}

void dbdata_save(dbdata_t* db) {
	dbio_write(DB_NAME, *db->elem, db->size);
}
