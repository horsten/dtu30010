/*SPACESHIP
 * buzzer_driver.h
 *
 *  Created on: 12. jan. 2024
 *      Author: Kåre
 */

#ifndef BUZZER_DRIVER_H
#define BUZZER_DRIVER_H
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"
#include "buzzer_sound.h"
#include "multtimer.h"

void buzzer_pin_setup(void);
void buzzer_time_setup(void);
void buzzer_setup(void);
void setFreq(uint16_t freq);

void melody(void);

#endif /* buzzer_driver.h */
