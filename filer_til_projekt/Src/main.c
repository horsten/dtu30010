//SPACESHIP
#include <multtimer.h> 			//multiplex timer init
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "escapekoder.h"
#include "asteroid.h"
volatile uint8_t flag;
int main(void)
{

	uart_init( 115200 ); // Initialize USB serial emulation at 9600 baud
	clrscr();
#if 1
	asteroid_t asteroid;
	init_timer();
	init_asteroid_val(&asteroid);
	//start_timer(0,200,1,testasto);
	draw_asteroid(&asteroid,10);
	//testasto();
#endif

#if 0// Timer multiplexer
	gotoyx(0, 0);
	printf("First this\n");
	init_timer();
	start_timer(0,200,0,testtimer1); // start_timer(
	start_timer(1,100,1,testtimer2);
	start_timer(2,700,1,testtimer3);
	activatecallback();

	while(1){}

#endif
}
