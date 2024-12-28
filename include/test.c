#include "array.h"

int main() {
	array_t arr;

	ARRAY_MAKE(&arr, int, 5);

	for (usize i = 0; i < array_size(&arr); i++) {
		ARRAY_SET(&arr, int, i, i);
	}

	for (usize i = 0; i < array_size(&arr); i++) {
		printf("Get -> %d\n", *ARRAY_GET(&arr, int, i));
	}
}
