#ifndef _ANSI_H_
#define _ANSI_H_

#include <stdint.h>

typedef struct {
	uint8_t initialized;
	uint16_t x_size;
	uint16_t y_size;
} terminfo_t;

void init_term();
const terminfo_t *get_terminfo(void);
void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void gotoxy(uint16_t x, uint16_t y);
void clrscr(void);
void underline(uint8_t on);
void hide_cursor(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void resetbgcolor(void);
void window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t style, const char *title);
void test_frame_update(void);

#endif
