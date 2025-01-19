#include "db.h"

#define DBIO_ENABLE_LOG
#include "dbio.h"

#include <memory.h>
#include <stdlib.h>

static inline void update_db_size(dbdata_t* db) {
	db->size = vec_mem_size(&db->data) + sizeof(usize);
}

void dbdata_make(dbdata_t* db, const char* dbname, dbdata_type_t type) {
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

void dbdata_push(dbdata_t* db, student_t* stu) {
	vec_push(&db->data, stu);
	update_db_size(db);
}

static inline void init_data_reigion_size(dbdata_t* db, byte* raw_byte) {
	memcpy(raw_byte, &db->size - sizeof(usize), sizeof(usize));
}

static void init_data_reigion(dbdata_t* db, byte* raw_byte) {
	for (u16 i = 0; i < vec_size(&db->data); i++) {
		memcpy(raw_byte + sizeof(usize) + (STUDENT_T_SIZE * i),
				VEC_GET(&db->data, student_t, i), STUDENT_T_SIZE);
	}
}

static byte* dbdata_to_byte(dbdata_t* db) {
	byte* raw_byte = malloc(db->size);
	ASSERT(raw_byte != NULL, DEF_ALLOC_ERRMSG);
	memset(raw_byte, 0, db->size);

	init_data_reigion_size(db, raw_byte);
	init_data_reigion(db, raw_byte);

	return raw_byte;
}

bool dbdata_save(dbdata_t* db) {
	if (db->type != SAVE) {
		return FALSE;
	}

	byte* raw_byte = dbdata_to_byte(db);
	bool status = dbio_write_fd(db->fd, raw_byte, db->size);
	free(raw_byte);
	return status;
}

// Fix incorrect size return
static inline bool get_data_reigion_size(dbdata_t* db, usize* buf) {
	return dbio_read_fd(db->fd, (byte*)buf, 0, sizeof(usize), 1);
}

static inline u16 calc_student_count(usize data_reigion_size) {
	return data_reigion_size / STUDENT_T_SIZE;
}

static inline bool get_data_reigion_bytes(dbdata_t* db,
		byte* buf, usize reigion_size) 
{
	return dbio_read_fd(db->fd, buf, sizeof(usize),
			STUDENT_T_SIZE, calc_student_count(reigion_size));
}

static bool byte_to_dbdata(dbdata_t* db) {
	usize data_reigion_size;
	if (!get_data_reigion_size(db, &data_reigion_size)) {
		return FALSE;
	}

	byte* data_reigion_bytes = malloc(data_reigion_size);
	ASSERT(data_reigion_bytes != NULL, DEF_ALLOC_ERRMSG);
	memset(data_reigion_bytes, 0, data_reigion_size);

	if (!get_data_reigion_bytes(db, data_reigion_bytes, data_reigion_size)) {
		return FALSE;
	}

	const u16 student_count = calc_student_count(data_reigion_size);
	for (u16 i = 0; i < student_count; i++) {
		vec_push(&db->data, data_reigion_bytes + (STUDENT_T_SIZE * i));
	}

	return TRUE;
}

bool dbdata_load(dbdata_t* db) {
	if (db->type != LOAD) {
		return FALSE;
	}
	if (!vec_empty(&db->data)) {
		vec_clear(&db->data);
	}

	return byte_to_dbdata(db);
}
