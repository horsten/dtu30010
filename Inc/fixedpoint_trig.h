#ifndef _FIXEDPOINT_TRIG_H_
#define _FIXEDPOINT_TRIG_H_ 1

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

// Warning: not reentrant, uses static buffer.
const char *print_fixed32_16_t(fixed32_16_t i);

fixed16_14_t sin_fp(uint16_t degree_base_512);
fixed16_14_t cos_fp(uint16_t degree_base_512);

fixed32_16_t sin_fp_32_16(uint16_t degree_base_512);
fixed32_16_t cos_fp_32_16(uint16_t degree_base_512);

typedef struct {
	fixed32_16_t x;
	fixed32_16_t y;
} vector_t;

void vector_rotate(vector_t *v, uint16_t degree_base_512);

void test_fp_sincos(void);

#endif
