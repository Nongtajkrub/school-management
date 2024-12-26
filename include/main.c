#include "list.h"

#define LIST_SIZE 15

int main() {
	list_t list;

	LIST_MAKE(&list, int);

	for (u8 i = 0; i < LIST_SIZE; i++) {
		LIST_APPEND(&list, int, i);

		printf("Tail -> %d\n", *LIST_TAIL(&list, int));
	}
}
