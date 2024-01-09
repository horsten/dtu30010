/*
 * timer.h
 *
 *  Created on: 6. jan. 2024
 *      Author: Lauri
 */

#ifndef TIMER_H_
#define TIMER_H_


#include <stdint.h>
#include <stdio.h>
void init_timer(void);
void timerstrucinit();
void newtime(void);
typedef struct {
	volatile int8_t h,m,s,hs;
} itstime;
void TIM1_BRK_TIM15_IRQHandler(void);
void drawtimer(void);
void timer_joystick(void);
#endif /* TIMER_H_ */
