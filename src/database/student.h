#pragma once

#define GET_DATABASE_SETTINGS

#include "../settings.h"

#include <type.h>

#define STUDENT_T_SIZE sizeof(student_t)
typedef struct {
	u32 id;
	char name[MAX_STUDENT_NAME_LEN];

	u8 age;
	u16 balance;
} student_t;

bool student_make(student_t* stu,
		u32 id, const char* name, u8 age, u16 balance);
