#include <multtimer.h> 			//multiplex timer init
#include <stdint.h>
#include <stdio.h>
#include "escapekoder.h"
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "asteroid.h"
#include "fixedpoint.h"


void init_asteroid_val(asteroid_t* a){
	a->pos.x = 100; //int16_to_fixed32_16(10);
	a->velocity.x = -1; //int16_to_fixed32_16(-1);
}

void asteroid_move(asteroid_t* a){
	a->pos.x += a->velocity.x;
}

void draw_asteroid(asteroid_t* a, uint8_t strtposy) {
	gotoyx(strtposy,a->pos.x);
	printf("a");
}
asteroid_t asteroid;
void testasto(asteroid_t* a){
	//asteroid_move(&asteroid);
	draw_asteroid(&asteroid,10);
}
