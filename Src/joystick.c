#include "joystick.h"
#include "30010_io.h"
#include "rgb_led.h"
#include <stdint.h>
#include <string.h>

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

joystick_status_t read_joystick(void)
{
	// bit 0: PA4: up
	// bit 1: PB0: down
	// bit 2: PC1: left
	// bit 3: PC0: right
	// bit 4: PB5: center
	joystick_status_t r;
	r.raw = 0;
	r.b.up=((GPIOA->IDR >> 4) & 1);
	r.b.down=((GPIOB->IDR >> 0) & 1);
	r.b.left=((GPIOC->IDR >> 1) & 1);
	r.b.right=((GPIOC->IDR >> 0) & 1);
	r.b.center=((GPIOB->IDR >> 5) & 1);
	return r;
}

void test_joystick(void)
{
	joystick_status_t v_old, v_new;
	int first = 1;
	init_joystick();
	while(1) {
		v_new = read_joystick();
		if (v_new.raw == v_old.raw || first) {
			first=0;
			printf("Joystick: UP: %d DOWN: %d LEFT: %d RIGHT: %d CENTER: %d\n",
					v_new.b.up, v_new.b.down, v_new.b.left, v_new.b.right, v_new.b.center);
		}
		v_old = v_new;
	};
}

void test_joystick_with_rgb_led(void)
{
	joystick_status_t v_old, v_new;
	v_old.raw = 0xff;
	init_joystick();
	init_rgb_led();
	printf("\nJoystick tester. Size of joystick_status_t: %d.\n", sizeof(joystick_status_t));
	while(1) {
		v_new = read_joystick();
		if (v_new.raw != v_old.raw) {
			// Set the LED depending on joystick state,
			// by choosing these values it's possible to get all
			// LED combinations.
			uint8_t led_val = (v_new.b.up ? 0x1 : 0) |  // UP: Red
						      (v_new.b.left ? 0x2 : 0) |  // LEFT: Green
						      (v_new.b.right ? 0x4 : 0) |  // RIGHT: Blue
						      (v_new.b.down ? 0x2 : 0) |  // DOWN: Green
						      (v_new.b.center ? 0x7 : 0);   // CENTER: Blue+Green+Red
			set_rgb_led(led_val);
			printf("Joystick: RAW: 0x%02x UP: %d DOWN: %d LEFT: %d RIGHT: %d CENTER: %d - LED: 0x%02x\n",
					v_new.raw, v_new.b.up, v_new.b.down, v_new.b.left, v_new.b.right, v_new.b.center, led_val);
		}
		v_old.raw = v_new.raw;
	};
}
