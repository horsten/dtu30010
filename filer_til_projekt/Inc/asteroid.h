/*
 * asteroid.h
 *
 *  Created on: 10. jan. 2024
 *      Author: Lauri
 */

#ifndef ASTEROID_H_
#define ASTEROID_H_
#include "fixedpoint.h"
typedef struct {
	vector_t pos;
	vector_t velocity;
} asteroid_t;

void asteroid_move(asteroid_t* a);
void draw_asteroid(asteroid_t* a, uint8_t strtpos);
void init_asteroid_val(asteroid_t* a);
void testasto(void);


#endif /* ASTEROID_H_ */
