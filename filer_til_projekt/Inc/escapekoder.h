/*
 * escapekoder.h
 *
 *  Created on: 9. jan. 2024
 *      Author: Lauri
 */

#ifndef ESCAPEKODER_H_
#define ESCAPEKODER_H_
#include "fixedpoint.h"
#include <stdint.h>
#include <stdio.h>

void clrscr();
void inverse(uint8_t on);
void gotoyx(uint8_t y, fixed32_16_t x);
#endif /* ESCAPEKODER_H_ */
