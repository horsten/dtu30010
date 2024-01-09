#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "timer.h"
#include "ansi.h"
#include "joystick.h"
itstime timer;
volatile uint8_t flag;
/*
 * timer.c */

void init_timer(void) {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 15;


	TIM15->CR1 = 0x0001; // Configure timer 15
	TIM15->ARR = 0xf9ff; // Set reload value to 63999
	TIM15->PSC = 0x0009; // Set prescale value to 9


	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0x0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void timerstrucinit(itstime *v) {
	v->h = 0;
	v->m = 0;
	v->s = 0;
	v->hs = 0;
}





	void TIM1_BRK_TIM15_IRQHandler(void) {
		flag = 1;

		TIM15->SR &= ~0x0001; // Clear interrupt bit

	}

void newtime(void) {
	timer.hs++;
	if (timer.hs >= 100) {
		timer.hs = 0;
		timer.s++;
	}

	if (timer.s >= 60) {
		timer.s = 0;

		timer.m++;
	}
	if (timer.m >= 60) {

		timer.m = 0;
		timer.h++;
	}

}
void drawtimer(void) {
	gotoxy(12,12);
	inverse(1);
	printf("Time since start: %02d:%02d:%02d:--",timer.h,timer.m,timer.s);
	inverse(0);

}








