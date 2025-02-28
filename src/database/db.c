#define DBIO_ENABLE_LOG

#include "db.h"
#include "dbio.h"

#include <memory.h>
#include <stdlib.h>

static inline u16 calc_student_count(database_t* db) {
	return db->data_reigion_size / STUDENT_T_SIZE;
}

static inline void update_db_size(database_t* db) {
	db->size = vec_mem_size(&db->data) + sizeof(usize);
	db->data_reigion_size = (db->size > 0) ? db->size - sizeof(usize) : 0;
	db->student_count = calc_student_count(db);
}

void database_make(database_t* db, const char* dbname, database_type_t type) {
	db->type = type;

	switch (type) {
	case LOAD:
		db->fd = dbio_make_rfd(dbname);
		break;
	case SAVE:
		db->fd = dbio_make_wfd(dbname);
		break;
	}

	VEC_MAKE(&db->data, student_t);
	update_db_size(db);
}

void database_destroy(database_t* db) {
	dbio_close_fd(db->fd);
	db->size = 0;
	vec_destroy(&db->data);
}

static void init_data_reigion_size(database_t* db, byte* raw_byte) {
	memcpy(raw_byte, (byte*)&db->data_reigion_size, sizeof(usize));
}

static void init_data_reigion(database_t* db, byte* raw_byte) {
	for (u16 i = 0; i < vec_size(&db->data); i++) {
		memcpy(raw_byte + sizeof(usize) + (STUDENT_T_SIZE * i),
				VEC_GET(&db->data, byte, i), STUDENT_T_SIZE);
	}
}

static byte* database_to_byte(database_t* db) {
	byte* raw_byte = malloc(db->size);
	ASSERT(raw_byte != NULL, DEF_ALLOC_ERRMSG);
	memset(raw_byte, 0, db->size);

	init_data_reigion_size(db, raw_byte);
	init_data_reigion(db, raw_byte);

	return raw_byte;
}

bool database_save(database_t* db) {
	if (db->type != SAVE) {
		DB_LOG(DB_WRONG_TYPE_ERRMSG);
		return false;
	}

	byte* raw_byte = database_to_byte(db);
	bool status = dbio_write_fd(db->fd, raw_byte, db->size);
	free(raw_byte);
	return status;
}

static inline bool get_data_reigion_size(database_t* db, usize* buf) {
	return dbio_read_fd(db->fd, (byte*)buf, 0, sizeof(usize), 1);
}

static inline bool get_data_reigion_bytes(database_t* db, byte* buf) {
	return dbio_read_fd(db->fd, buf, sizeof(usize),
			STUDENT_T_SIZE, db->student_count);
}

static bool byte_to_database(database_t* db) {
	if (!get_data_reigion_size(db, &db->data_reigion_size)) {
		return false;
	}

	db->student_count = calc_student_count(db);

	byte* data_reigion_bytes = malloc(db->data_reigion_size);
	ASSERT(data_reigion_bytes != NULL, DEF_ALLOC_ERRMSG);
	memset(data_reigion_bytes, 0, db->data_reigion_size);

	if (!get_data_reigion_bytes(db, data_reigion_bytes)) {
		return false;
	}

	for (u16 i = 0; i < db->student_count; i++) {
		vec_push(&db->data, data_reigion_bytes + (STUDENT_T_SIZE * i));
	}

	return true;
}

bool database_load(database_t* db) {
	if (db->type != LOAD) {
		DB_LOG(DB_WRONG_TYPE_ERRMSG);
		return false;
	}
	if (!vec_empty(&db->data)) {
		vec_clear(&db->data);
	}

	return byte_to_database(db);
}

bool database_push(database_t* db, student_t* stu) {
	if (db->type != SAVE) {
		DB_LOG(DB_WRONG_TYPE_ERRMSG);
		return false;
	}

	vec_push(&db->data, stu);
	update_db_size(db);
	return true;
}

i32 database_id_by_name(database_t* db, const char* name) {
	for (u16 i = 0; i < db->student_count; i++) {
		const student_t* stu = VEC_GET(&db->data, student_t, i);

		if (strcmp(stu->name, name) == 0) {
			return stu->id;
		}
	}

	return -1;
}

const student_t* database_student_by_id(database_t* db, u16 id) {
	for (u16 i = 0; i < db->student_count; i++) {
		const student_t* stu = VEC_GET(&db->data, student_t, i);

		if (stu->id == id) {
			return stu;
		}
	}

	return NULL;
}
