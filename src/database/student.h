#pragma once

#include <type.h>

#define MAX_STUDENT_NAME_LEN 128

#define STUDENT_T_SIZE sizeof(student_t)
typedef struct {
	u32 id;
	char name[MAX_STUDENT_NAME_LEN];

	u8 age;
	u16 balance;
} student_t;

void student_make(student_t* stu,
		u32 id, const char* name, u8 age, u16 balance);
