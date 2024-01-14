#define ESC 0x1B
#include "escapekoder.h"
void clrscr() {
	printf("%c[2J", ESC); // Clears terminal
}


void inverse(uint8_t on) {
	printf("%c[%dm", ESC, on ? 7 : 27);
}

void gotoyx(uint8_t y, fixed32_16_t x) {
		printf("%c[%d;%ldH", ESC, y, x);
	}

