#include "joystick.h"
#include "30010_io.h"
#include "rgb_led.h"
#include <stdint.h>

uint8_t read_joystick(void)
{
	// bit 0: PA4: up
	// bit 1: PB0: down
	// bit 2: PC1: left
	// bit 3: PC0: right
	// bit 4: PB5: center
	return ((GPIOA->IDR >> 4) & 1) << 0 |
		   ((GPIOB->IDR >> 0) & 1) << 1 |
		   ((GPIOC->IDR >> 1) & 1) << 2 |
		   ((GPIOC->IDR >> 0) & 1) << 3 |
		   ((GPIOB->IDR >> 5) & 1) << 4;
}

void init_joystick(void)
{
	// Joystick pins:
	// PC0, PA4, PB5, PC1, PB0

	// Enable clock for GPIO Port A, B, C
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC;

	// Set pin PA4 to input, no pull-up/down
	GPIOA->MODER &= ~(0x3 << (4*2)); // Set mode register for PA4 to 0x00 = INPUT
	GPIOA->PUPDR &= ~(0x00000003 << (4 * 2)); // Clear push/pull register

	// Now PB0 and PB5
	GPIOB->MODER &= ~(0x3 << (4*2));
	GPIOB->MODER &= ~(0x3 << (5*2));
	GPIOB->PUPDR &= ~(0x00000003 << (4 * 2));
	GPIOB->PUPDR &= ~(0x00000003 << (5 * 2));

	// Now PC0 and PC1
	GPIOC->MODER &= ~(0x3 << (0*2));
	GPIOC->MODER &= ~(0x3 << (1*2));
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2));
	GPIOC->PUPDR &= ~(0x00000003 << (1 * 2));
}

void test_joystick(void)
{
	uint8_t v_old=0xff, v_new;
	init_joystick();
	while(1) {
		v_new = read_joystick();
		if (v_new != v_old) {
			printf("Joystick: UP: %d DOWN: %d LEFT: %d RIGHT: %d CENTER: %d\n",
					(v_new >> 0) & 1,
					(v_new >> 1) & 1,
					(v_new >> 2) & 1,
					(v_new >> 3) & 1,
					(v_new >> 4) & 1);
		}
		v_old = v_new;
	};
}

void test_joystick_with_rgb_led(void)
{
	uint8_t v_old=0xff, v_new;
	init_joystick();
	init_rgb_led();
	while(1) {
		v_new = read_joystick();
		if (v_new != v_old) {
			// Set the LED depending on joystick state,
			// by choosing these values it's possible to get all
			// LED combinations.
			uint8_t led_val = ((v_new & 0x01) ? 0x1 : 0) |  // UP: Red
						      ((v_new & 0x04) ? 0x2 : 0) |  // LEFT: Green
						      ((v_new & 0x08) ? 0x4 : 0) |  // RIGHT: Blue
						      ((v_new & 0x02) ? 0x2 : 0) |  // DOWN: Green
						      ((v_new & 0x10) ? 0x7 : 0);   // CENTER: Blue+Green+Red
			set_rgb_led(led_val);
			printf("Joystick: UP: %d DOWN: %d LEFT: %d RIGHT: %d CENTER: %d - LED: 0x%02x\n",
					(v_new >> 0) & 1,
					(v_new >> 1) & 1,
					(v_new >> 2) & 1,
					(v_new >> 3) & 1,
					(v_new >> 4) & 1,
					led_val);
		}
		v_old = v_new;
	};
}
