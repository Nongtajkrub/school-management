#include "network/server/server.h"
#include "network/client/client.h"
#include "gui/gui.h"

#include <type.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_ARG_ERR_MSG \
	"Invalid program argument"

#define SERV_SELC_ARG "s"
#define SERV_SELC 0

#define CLI_SELC_ARG "c"
#define CLI_SELC 1

#define INVALID_SELC 2

static u8 check_arg(int argc, char* argv[]) {
	if (argc == 1) {
		perror(INVALID_ARG_ERR_MSG);
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], SERV_SELC_ARG) == 0) {
		return SERV_SELC;
	}
	if (strcmp(argv[1], CLI_SELC_ARG) == 0) {
		return CLI_SELC;
	}
	return INVALID_SELC;
}

int main(int argc, char* argv[]) {
	switch (check_arg(argc, argv)) {
	case SERV_SELC:
		serv_main();
		break;
	case CLI_SELC:
		cli_main();
		break;
	default:
		printf(INVALID_ARG_ERR_MSG);
		exit(EXIT_FAILURE);
	} 
}
