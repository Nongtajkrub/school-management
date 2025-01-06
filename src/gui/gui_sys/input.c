#include "input.h"

#include <type.h>
#include <stdio.h>

char* ui_input_get(const char* promt) {
	char* input = NULL;
	size_t len = 0;

	printf("%s -> ", promt);
	getline(&input, &len, stdin);

	return input;
}
