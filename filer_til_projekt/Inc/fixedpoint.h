/*
 * fixedpoint.h
 *
 *  Created on: 10. jan. 2024
 *      Author: Lauri
 */

#ifndef FIXEDPOINT_H_
#define FIXEDPOINT_H_


#include <stdint.h>

typedef int16_t fixed16_14_t;
typedef int32_t fixed32_16_t;

inline __attribute__((always_inline)) fixed32_16_t fixed16_14_to_fixed32_16(fixed16_14_t v)
{
	return v << 2;
}

inline __attribute__((always_inline)) fixed32_16_t int16_to_fixed32_16(int16_t v)
{
	return v << 16;
}

inline __attribute__((always_inline)) fixed32_16_t fixed32_16_mul(fixed32_16_t x, fixed32_16_t y)
{
	return (int64_t)(x * (int64_t)(y) << 16) >> 32;
}

inline  __attribute__((always_inline)) int16_t round_fixed32_16(fixed32_16_t v)
{
	return (v >> 16) + ((v >> 15) & 1);
}

// Warning: not reentrant, uses static buffer.
const char *print_fixed32_16_t(fixed32_16_t i);


typedef struct {
	fixed32_16_t x;
	fixed32_16_t y;
} vector_t;







#endif /* FIXEDPOINT_H_ */
