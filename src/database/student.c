#include "student.h"

#include <stdlib.h>
#include <string.h>

void student_make(student_t* stu,
		u32 id, const char* name, u8 age, u16 balance) 
{
	stu->id = id;

	const usize name_size = strlen(name);
	
	ASSERT(name_size < MAX_STUDENT_NAME_LEN - 1, DEF_OVERFLOW_ERRMSG);
	memset(stu->name, '\0', MAX_STUDENT_NAME_LEN);
	memcpy(stu->name, name, name_size);

	stu->age = age;
	stu->balance = balance;
}
