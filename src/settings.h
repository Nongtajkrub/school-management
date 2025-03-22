// no pragma once becareful

#ifdef GET_SERVER_SETTINGS
	#define ADDR "127.0.0.1"
	#define PORT 8080 
#endif // #ifdef GET_SERVER_INFO

#ifdef GET_DATABASE_SETTINGS
	#define MAX_STUDENT_NAME_LEN 128 
	#define CHUNK_BUF_SIZE 32
	#define CHUNK_SIZE CHUNK_BUF_SIZE * DATABASE_BLOCK_SIZE 
#endif // #ifdef GET_DATABASE_SETTINGS
