#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_ 1

#include <stdint.h>

typedef union  {
	// bit 0: PA4: up
	// bit 1: PB0: down
	// bit 2: PC1: left
	// bit 3: PC0: right
	// bit 4: PB5: center
	struct {
		uint8_t up:1;
		uint8_t down:1;
		uint8_t left:1;
		uint8_t right:1;
		uint8_t center:1;
	} b; // "b" for "Bitfield" :)
	uint8_t raw;
} joystick_status_t;

void init_joystick(void);
joystick_status_t read_joystick(void);
void test_joystick(void);
void test_joystick_with_rgb_led(void);

#endif
