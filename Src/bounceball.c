#include "bounceball.h"
#include "ansi.h"
#include "fixedpoint_trig.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	uint16_t xsize;
	uint16_t ysize;
	uint16_t xoffset;
	uint16_t yoffset;
	uint16_t status_x;
	uint16_t status_y;
} playfield_t;

typedef struct {
	vector_t v; // velocity
	vector_t p; // position
} ball_t;

static void usleep(uint32_t microseconds)
{
	for (int c=0; c<microseconds; c++) {
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
 		asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	}
}

static void update_pos(ball_t *b)
{
	b->p.x += b->v.x;
	b->p.y += b->v.y;
}

static void bounce(playfield_t *pf)
{
	ball_t b;
	int hits=0;
	// Start position: 0,0
	b.p.x = int16_to_fixed32_16(0);
	b.p.y = int16_to_fixed32_16(0);
	// Start velocity: 1.25, 0.75
	//b.v.x = (1 << 16) | 0x8000;
	//b.v.x = (1 << 16) | 0xa000;
	// Start velocity: 1,1
	b.v.x = int16_to_fixed32_16(1);//0x5a82 >> 1; // sqrt(2)/2 //int16_to_fixed32_16(1);
	b.v.y = int16_to_fixed32_16(1);//0x5a82 >> 1; // sqrt(2)/2 //int16_to_fixed32_16(1);
	while(1) {
		// Update hit counter
		gotoxy(pf->xoffset+pf->status_x, pf->xoffset+pf->status_y);
		color(11,4);
		printf("Hits: %4d\n", hits);

		int16_t rounded_x, rounded_y;
		rounded_x = round_fixed32_16(b.p.x);
		rounded_y = round_fixed32_16(b.p.y);

		int obscured = 0;
		// Check if the ball is obscured by the infobox
		if ( (rounded_x >= pf->status_x-3) && (rounded_x <= pf->status_x-2+14) &&
			 (rounded_y >= pf->status_y-1) && (rounded_y <= pf->status_y-1+3))
			obscured = 1;

		// Draw the ball
		if (!obscured) {
			gotoxy(pf->xoffset+rounded_x, pf->yoffset+rounded_y);
			color(13,7);
			printf("\xfe");
		}

		// Wait for a bit
		usleep(20000);

		// Now delete the ball from the old position
		if (!obscured) {
			gotoxy(pf->xoffset+rounded_x, pf->yoffset+rounded_y);
			printf(" ");
		}

		// Update ball's position
		update_pos(&b);
		int16_t newx, newy;
		newx = round_fixed32_16(b.p.x);
		newy = round_fixed32_16(b.p.y);

		// Check for collission with walls
		if (newx<0 || newx >= pf->xsize-1) {
			b.v.x = -b.v.x;
			update_pos(&b);
			newx = round_fixed32_16(b.p.x);
			newy = round_fixed32_16(b.p.y);
			hits++;
		}
		if (newy<0 || newy >= pf->ysize-1) {
			b.v.y = -b.v.y;
			update_pos(&b);
			newx = round_fixed32_16(b.p.x);
			newy = round_fixed32_16(b.p.y);
			hits++;
		}
		// Check for collision with infobox
	}
}

void bounceball(void)
{
	playfield_t pf;

	const terminfo_t *ti = get_terminfo();
	if (!ti->initialized) {
		init_term();
	}
	pf.xsize = (ti->x_size-4);
	pf.ysize = (ti->y_size-4);
	pf.xoffset=2;
	pf.yoffset=2;
	pf.status_x = (pf.xsize >> 1) - (12 >> 1); // " Hits: #### ": 12 chars
	pf.status_y = pf.ysize >> 1;
	printf(" Playfield size: %dx%d. Let's Bounce!", pf.xsize, pf.ysize);
	hide_cursor(1);
	window(pf.xoffset-1,pf.yoffset-1,pf.xsize+2,pf.ysize+2,2,NULL);
	window(pf.xoffset+pf.status_x-2,pf.yoffset+pf.status_y-1,14,3,3,NULL);
	bounce(&pf);
}
