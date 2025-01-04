#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "keyboardio.h"

int main() {
    printf("Press any key to see its value. Press 'q' to quit.\n");

    while (1) {
		i32 ch = kbio_check_input();

        if (ch != EOF) {
            printf("You pressed: %c (ASCII: %d)\n", ch, ch);
            if (ch == 'q') break;
		}
    }

    return 0;
}
