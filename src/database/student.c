#include "student.h"

#include <stdlib.h>
#include <string.h>

bool student_make(student_t* stu,
		u32 id, const char* name, u8 age, u16 balance) 
{
	const usize name_size = strlen(name);
	
	if (name_size >= MAX_STUDENT_NAME_LEN - 1) {
		return FALSE;
	}

	memset(stu->name, '\0', MAX_STUDENT_NAME_LEN);
	memcpy(stu->name, name, name_size);

	stu->id = id;
	stu->age = age;
	stu->balance = balance;

	return TRUE;
}
