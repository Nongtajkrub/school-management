#pragma once

#include <type.h>

#define STUDENT_T_SIZE sizeof(student_t)
typedef struct {
	u32 id;
	const char* name;

	u8 age;
} student_t;

void student_make(student_t* stu, u32 id, const char* name, u8 age);
