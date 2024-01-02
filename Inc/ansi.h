#ifndef _ANSI_H_
#define _ANSI_H_

#include <stdint.h>

void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void gotoxy(uint8_t x, uint8_t y);
void clrscr(void);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void resetbgcolor(void);
void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t style, const char *title);

#endif
