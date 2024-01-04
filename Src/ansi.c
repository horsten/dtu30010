#include "ansi.h"
#include "30010_io.h"
#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <stdlib.h>

#ifdef ESC
#undef ESC
#endif

#define ESC "\x1b"
#define CSI ESC "["

const terminfo_t *get_terminfo(void) {
	static terminfo_t the_terminfo = {
			.initialized = 0,
			.x_size = 0,
			.y_size = 0
	};
	return (const terminfo_t *)&the_terminfo;
}

int read_uart_timeout_breakchar(uint32_t timeout, char *rxbuf, uint16_t rxbuflen, uint8_t breakchar)
{
	int rx = 0;
	int tt;
	if (rxbuflen == 0)
		return 0;
	do {
		tt = timeout;
		while (uart_get_count() == 0 && tt--)
			;
		if (uart_get_count() == 0)
			return rx;
		rxbuf[rx] = uart_get_char();
		if (rxbuf[rx++] == breakchar)
			return rx;
	} while(tt && rx < rxbuflen);
	return rx;
}

void init_term(void)
{
	terminfo_t *ti = (terminfo_t *)get_terminfo();
	// Drain input buffer
	while (uart_get_count())
		uart_get_char();

	// Soft Terminal Reset, Reset Colors, Clear Screen, Cursor at 999/999, Report Curser Position
	printf(/* CSI "!p" */ CSI "0m" CSI "2J" CSI "999;999H" CSI "6n" );

	// Read from serial port until a response ending with R is received or timeout
	char buf[32];
	int r = read_uart_timeout_breakchar(10000, buf, sizeof(buf)-1, 'R');
	// Parse the response, it should look like ESC [ rows ; cols R
	do {
		if (r < 1 || buf[r-1] != 'R')
			break;
		buf[r] = '\0';
		char *s = rindex(buf, '\x1b');
		if (!s || !*s || *++s != '[')
			break;
		s = strtok(++s, ";");
		if (!s)
			break;
		uint16_t rows = atoi(s);
		if (rows < 1)
			break;
		s = strtok(NULL, "R");
		if (!s)
			break;
		uint16_t cols = atoi(s);
		if (cols < 1)
			break;
		printf("\n" CSI "HTerminal size detected: %dx%d. ", cols, rows);
		ti->x_size = cols;
		ti->y_size = rows;
		ti->initialized = 1;
		return;
	} while(0);

	printf("\n" CSI "HFailed to detect terminal size. Assuming 80x24. ");
	ti->x_size = 80;
	ti->y_size = 24;
	ti->initialized = 1;
}

void fgcolor(uint8_t foreground)
{
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf(ESC "[%d;%dm", type, foreground+30);
}

void bgcolor(uint8_t background)
{
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf(ESC "[%dm", background+40);
}

void color(uint8_t foreground, uint8_t background)
{
// combination of fgcolor() and bgcolor() - uses less bandwidth
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf(ESC "[%d;%d;%dm", type, foreground+30, background+40);
}

void resetbgcolor()
{
// gray on black text, no underline, no blink, no reverse
  printf(ESC "[m");
}

void clrscr()
{
	printf(ESC "[H" ESC "[2J");
}

void hide_cursor(uint8_t on)
{
	printf(CSI "?25%c", on ? 'l' : 'h');
}

void underline(uint8_t on)
{
	printf(ESC "[%sm", on ? "4" : "24");
}

void blink(uint8_t on)
{
	printf(ESC "[%sm", on ? "5" : "25");
}

void inverse(uint8_t on)
{
	printf(ESC "[%sm", on ? "7" : "27");
}

void gotoxy(uint16_t x, uint16_t y)
{
	printf(ESC "[%d;%df", y+1, x+1);
}

// Line drawing characters CP437
#define L_TL "\xbf" // Top left single
#define L_TR "\xda" // Top right single
#define L_BL "\xc0" // Bottom right single
#define L_BR "\xd9" // Bottom leftsingle
#define L_H  "\xc4" // Horizontal line
#define L_V  "\xb3" // Horizontal line

#define L_TL_D "\xbb" // Top left double
#define L_TR_D "\xc9" // Top right double
#define L_BL_D "\xc8" // Bottom right double
#define L_BR_D "\xbc" // Bottom left double
#define L_H_D "\xcd" // Horizontal line double
#define L_V_D "\xba" // Horizontal line double

typedef struct {
	const char *tl;
	const char *tr;
	const char *br;
	const char *bl;
	const char *h;
	const char *v;
	uint8_t no_title : 1;
	uint8_t fgcol;
	uint8_t bgcol;
	uint8_t titlefg;
	uint8_t titlebg;
} window_style_t;

static const window_style_t window_styles[] = {
		{ .tl=L_TL, .tr=L_TR, .bl=L_BL, .br=L_BR, .h=L_H, .v=L_V, .fgcol=15, .bgcol=4, .titlefg=0, .titlebg=7},
		{ .tl=L_TL_D, .tr=L_TR_D, .bl=L_BL_D, .br=L_BR_D, .h=L_H_D, .v=L_V_D, .fgcol=0, .bgcol=7, .titlefg=15, .titlebg=1 },
		{ .tl=L_TL_D, .tr=L_TR_D, .bl=L_BL_D, .br=L_BR_D, .h=L_H_D, .v=L_V_D, .fgcol=0, .bgcol=7, .no_title=1 },
		{ .tl=L_TL, .tr=L_TR, .bl=L_BL, .br=L_BR, .h=L_H, .v=L_V, .fgcol=15, .bgcol=4, .no_title=1 }
};

void window(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint8_t style, const char *title)
{
	char *titlebuf = NULL;
	int title_len;
	const window_style_t *st;
	if (style<sizeof(window_styles)/sizeof(window_style_t)) {
		st = &window_styles[style];
	} else {
		st = &window_styles[0];
	}

	if (st->no_title) {
		title_len = 0;
	} else {
		title_len = strlen(title);
		if (title_len > width-x1-6) {
			title_len = width-x1-6;
			if (title_len < 1) {
				title_len = 0;
			}
		}
		if (title_len > 1) {
			titlebuf = alloca(title_len+1);
			strncpy(titlebuf, title, title_len);
			titlebuf[title_len-1]='\xaf';
			titlebuf[title_len]='\0';
		} else {
			title_len = 0;
		}
	}
	gotoxy(x1,y1);
	color(st->fgcol, st->bgcol);
	// Draw corner and 1 line
	printf(st->tr);
	if (title_len) {
		color(st->titlefg, st->titlebg);
		printf(" %s ", titlebuf);
		color(st->fgcol, st->bgcol);
		title_len+=2;
	}
	for (int c=0; c<width-title_len-2; c++) {
		printf(st->h);
	}
	printf(st->tl);
	for (int y=y1+1; y<y1+height-1; y++) {
		gotoxy(x1, y);
		printf(st->v);
		for (int x=x1+1; x<x1+width-1; x++) {
			printf(" ");
		}
		printf(st->v);
	}
	gotoxy(x1,y1+height-1);
	printf(st->bl);
	for (int c=0; c<width-2; c++) {
		printf(st->h);
	}
	printf(st->br);
}
