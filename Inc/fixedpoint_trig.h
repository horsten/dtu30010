#ifndef _FIXEDPOINT_TRIG_H_
#define _FIXEDPOINT_TRIG_H_ 1

#include <stdint.h>

typedef int16_t fixed16_14;
typedef int32_t fixed32_16;

inline __attribute__((always_inline)) fixed32_16 fixed16_14_to_fixed32_16(fixed16_14 v)
{
	return v << 2;
}

// Warning: not reentrant, uses static buffer.
const char *print_fixed32_16(fixed32_16 i);

fixed16_14 sin_fp(uint16_t degree_base_512);
fixed16_14 cos_fp(uint16_t degree_base_512);

void test_fp_sincos(void);

#endif
