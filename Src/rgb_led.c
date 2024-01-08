#include "rgb_led.h"
#include "30010_io.h"

void init_rgb_led(void)
{
	// LED pins:
	// PA9, PC7, PB4

	// Enable clock for GPIO Port A, B, C
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC;

	// Set pin PA9 to output, set it high to turn the LED off
	GPIOA->ODR |= (1 << 9);
	GPIOA->MODER = (GPIOA->MODER & ~(0x3 << (9*2))) | (0x1 << (9*2));
	GPIOA->OTYPER &= ~(0x0 << 9); // type 0 = push/pull

	// Now PB4
	GPIOB->ODR |= (1 << 4);
	GPIOB->MODER = (GPIOB->MODER & ~(0x3 << (4*2))) | (0x1 << (4*2));
	GPIOB->OTYPER &= ~(0x0 << 4); // type 0 = push/pull

	// Now PC7
	GPIOC->ODR |= (1 << 7);
	GPIOC->MODER = (GPIOC->MODER & ~(0x3 << (7*2))) | (0x1 << (7*2));
	GPIOC->OTYPER &= ~(0x0 << 7); // type 0 = push/pull
}

void set_rgb_led(uint8_t val)
{
	val ^= 0x7;
	GPIOB->ODR = (GPIOB->ODR & ~(1<<4)) | (((val >> 0) & 1) << 4);
	GPIOC->ODR = (GPIOC->ODR & ~(1<<7)) | (((val >> 1) & 1) << 7);
	GPIOA->ODR = (GPIOA->ODR & ~(1<<9)) | (((val >> 2) & 1) << 9);
}

void test_rgb_led(void)
{
	int c;
	init_rgb_led();
	for(;;) {
		for (c=0; c<8; c++) {
			printf("Set LED value: 0x%02x = Binary %d%d%d\n", c,
					(c >> 2)&1,
					(c >> 1)&1,
					(c >> 0)&1
					);
			set_rgb_led(c);
			for (int q=0; q<1000000; q++)
				asm volatile("nop");
		}
	}
}
