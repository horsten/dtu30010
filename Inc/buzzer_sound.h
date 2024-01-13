/*SPACESHIP
 * buzzer_sound.h
 *
 *  Created on: 12. jan. 2024
 *      Author: Kåre
 */

#ifndef BUZZER_SOUND_H
#define BUZZER_SOUND_H
#include <stdint.h>
#include <stdio.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"
#include "buzzer_driver.h"
#include "multtimer.h"
#include "buzzer_sound.h"

void pitch600(void);
void pitch550(void);
void pitch500(void);
void silent(void);

void melody(void);

#endif /* buzzer_sound.h */
