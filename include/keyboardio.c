#include "keyboardio.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

i32 kbio_ch = -1;

bool kbio_check_input() {
	struct termios oldt,
				   newt;
	
	// get current terminal flag
	i32 oldf = fcntl(STDIN_FILENO, F_GETFL, 0); 

	// get current terminal setting
	tcgetattr(STDIN_FILENO, &oldt);

	// get a copy of the current terminal setting and modifie them
	newt = oldt;

	// make input get process immedietly no need for a new line
	// disble echo character that is type
	newt.c_lflag &= ~(ICANON | ECHO); 

	// apply the new terminal setting
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// make the terminal none blocking
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	// get the keyboard input
	kbio_ch = getchar();

	// restore terminal setting
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

	// return true if a key is press else false
	return (kbio_ch != EOF);
}
