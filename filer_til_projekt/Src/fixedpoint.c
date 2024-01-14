#include "fixedpoint.h"
#include <stdio.h>

const char *print_fixed32_16_t(fixed32_16_t i) {
	static char buf[4][16];
	static uint8_t buf_in_use = 0;
	int negative = 0;
	// Prints a signed 16.16 fixed point number
	if ((i & 0x80000000) != 0) { // Handle negative numbers
		negative = 1;
		i = ~i + 1;
	}
	// rotate through 4 buffers so we can have up to 4 uses in a single printf call...
	// bit quick and dirty but it works.
	if (buf_in_use > 3) {
		buf_in_use = 0;
	}
	sprintf(buf[buf_in_use], "%s%ld.%04ld", negative ? "-" : "", i >> 16, 10000 * (uint32_t)(i & 0xFFFF) >> 16);
	return (const char *)&(buf[buf_in_use++]);
}
