#pragma once

#ifdef GET_SERVER_SETTINGS
	#define ADDR "127.0.0.1"
	#define PORT 8080 
#endif // #ifdef GET_SERVER_INFO

#ifdef GET_DATABASE_SETTINGS
	#define MAX_STUDENT_NAME_LEN 128
#endif // #ifdef GET_DATABASE_SETTINGS
