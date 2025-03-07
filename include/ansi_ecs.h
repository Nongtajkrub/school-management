/*
 * Generated by ChatGPT
*/

#pragma once

// foreground text color
#define ANSI_ESC_BLACK_F   "\033[30m"
#define ANSI_ESC_RED_F     "\033[31m"
#define ANSI_ESC_GREEN_F   "\033[32m"
#define ANSI_ESC_YELLOW_F  "\033[33m"
#define ANSI_ESC_BLUE_F    "\033[34m"
#define ANSI_ESC_MAGENTA_F "\033[35m"
#define ANSI_ESC_CYAN_F    "\033[36m"
#define ANSI_ESC_WHITE_F   "\033[37m"

// background text color
#define ANSI_ESC_BLACK_B   "\033[40m"
#define ANSI_ESC_RED_B     "\033[41m"
#define ANSI_ESC_GREEN_B   "\033[42m"
#define ANSI_ESC_YELLOW_B  "\033[43m"
#define ANSI_ESC_BLUE_B    "\033[44m"
#define ANSI_ESC_MAGENTA_B "\033[45m"
#define ANSI_ESC_CYAN_B    "\033[46m"
#define ANSI_ESC_WHITE_B   "\033[47m"

// text styles
#define ANSI_ESC_BOLD             "\033[1m"
#define ANSI_ESC_DIM              "\033[2m"
#define ANSI_ESC_ITALIC           "\033[3m"
#define ANSI_ESC_UNDERLINE        "\033[4m"
#define ANSI_ESC_BLINK            "\033[5m"
#define ANSI_ESC_REVERSED         "\033[7m"
#define ANSI_ESC_HIDDEN           "\033[8m"
#define ANSI_ESC_STRIKETHROUGH    "\033[9m"
#define ANSI_ESC_DOUBLE_UNDERLINE "\033[21m"
#define ANSI_ESC_OVERLINE         "\033[53m"

// reset Code
#define ANSI_ESC_RESET "\033[0m"

// cursors
#define ANSI_ESC_CURSOR_HOME "\033[H"
#define ANSI_ESC_CURSOR_HIDE "\033[?25l"
#define ANSI_ESC_CURSOR_SHOW "\033[?25h"

// terminal
#define ANSI_ESC_CLEAR_TERM "\033[2J"
