#include "student.h"

void student_make(student_t* stu, u32 id, const char* name, u8 age) {
	stu->id = id;
	stu->name = name;
	stu->age = age;
}
