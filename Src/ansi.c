#include "ansi.h"
#include <stdio.h>
#include <string.h>
#include <alloca.h>

#ifdef ESC
#undef ESC
#endif

#define ESC "\x1b"

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
	printf(ESC "[2J");
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

void gotoxy(uint8_t x, uint8_t y)
{
	printf(ESC "[%d;%df", y, x);
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
	uint8_t fgcol;
	uint8_t bgcol;
	uint8_t titlefg;
	uint8_t titlebg;
} window_style_t;

static const window_style_t window_styles[] = {
		{ .tl=L_TL, .tr=L_TR, .bl=L_BL, .br=L_BR, .h=L_H, .v=L_V, .fgcol=15, .bgcol=4, .titlefg=11, .titlebg=7},
		{ .tl=L_TL_D, .tr=L_TR_D, .bl=L_BL_D, .br=L_BR_D, .h=L_H_D, .v=L_V_D, .fgcol=0, .bgcol=7, .titlefg=15, .titlebg=1 }
};

void window(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t style, const char *title)
{
	char *titlebuf = NULL;
	int title_len = strlen(title);
	const window_style_t *st;
	if (style<sizeof(window_styles)/sizeof(window_style_t)) {
		st = &window_styles[style];
	} else {
		st = &window_styles[0];
	}

	if (title_len > x2-x1-6) {
		title_len = x2-x1-6;
		if (title_len < 1) {
			title_len = 0;
		}
	}
	if (title_len > 0) {
		titlebuf = alloca(title_len+1);
		strncpy(titlebuf, title, title_len);
		titlebuf[title_len]='\0';
	}
	gotoxy(x1,y1);
	color(st->fgcol, st->bgcol);
	// Draw corner and 1 line
	printf(st->tr);
	if (title_len) {
		color(st->titlefg, st->titlebg);
		printf(" %s ", titlebuf);
		color(st->fgcol, st->bgcol);
	}
	for (int c=x1+title_len+4; c<x2; c++) {
		printf(st->h);
	}
	printf(st->tl);
	for (int y=y1+1; y<y2; y++) {
		gotoxy(x1, y);
		printf(st->v);
		for (int x=x1+1; x<x2-1; x++) {
			printf(" ");
		}
		printf(st->v);
	}
	gotoxy(x1,y2);
	printf(st->bl);
	for (int x=x1+1; x<x2-1; x++) {
		printf(st->h);
	}
	printf(st->br);
}
