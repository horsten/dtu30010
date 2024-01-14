/*SPACESHIP
 * timer.h
 *
 *  Created on: 9. jan. 2024
 *      Author: Lauri
 */

#ifndef MULTTIMER_H_
#define MULTTIMER_H_
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"

void activatecallback(void);

void start_timer(uint8_t timindex, uint32_t time_ms,uint8_t repeating, void (*callback)(void));

void init_timer(void);




void testtimer1(void);
void testtimer2(void);
void testtimer3(void);
#endif /* MULTTIMER_H_ */
