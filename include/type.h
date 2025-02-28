#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define ASSERT_ENABLE

#ifdef ASSERT_ENABLE
	#define ASSERT(COND, MSG)                                                 \
			if (!(COND)) {                                                    \
				fprintf(stderr,                                               \
					"Assertion failed: (%s), function %s, file %s, line %d.\n"\
					"Message: %s\n",                                          \
					#COND, __func__, __FILE__, __LINE__, MSG);                \
				exit(EXIT_FAILURE);                                           \
			}                                                                 
#else
	#define ASSERT(COND, MSG) ;;
#endif // #ifdef ASSERT_ENABLE

#define DEF_ALLOC_ERRMSG \
		"Fail to allocate memory"
#define DEF_OVERFLOW_ERRMSG \
		"Buffer overflow"
#define DEF_NOT_IMPLEMENTED_ERRMSG \
		"Not implemented!"
#define NO_ERRMSG \
		"No message"

#define MAX_I32_DIGIT 11
#define MAX_F32_DIGIT 8 

#define CHAR_TO_INT(CHAR) (CHAR - '0')
#define INT_TO_CAHR(INT) (INT + '0')

#include <stdio.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef unsigned int uint;

typedef ssize_t ssize;
typedef size_t usize;

typedef char byte;

typedef float f32;
typedef double f64;

typedef void (*void_func)(void*);
typedef bool (*bool_func)(void*);

typedef enum : u8 {
	TYPE_UNKNOWN,
	TYPE_INT,
	TYPE_STRING,
	TYPE_BOOL,
	TYPE_FLOAT
} data_types_t;
