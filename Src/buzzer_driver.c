// For at bruge trenger man bare at kalde på buzzer_setup() og setFreq(), med ønsket pitch

#include <buzzer_driver.h>
#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "buzzer_driver.h"
#include "multtimer.h"
volatile uint8_t flag;
/*
 * buzzer_driver.c */

void buzzer_pin_setup(void){						// pretty much descriptive name, code copied from exercises and changed parameters
	// Set pin PB10 to output
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB;

	GPIOB->OSPEEDR &= ~(0x00000003 << (10 * 2)); // Clear speed register
	GPIOB->OSPEEDR |= (0x00000002 << (10 * 2)); // set speed register (0x01 - 10 MHz, 0x02 - 2 MHz, 0x03 - 50 MHz)
	GPIOB->OTYPER &= ~(0x0001 << (10 * 1)); // Clear output type register
	GPIOB->OTYPER |= (0x0000 << (1)); // Set output type register (0x00 - Push pull, 0x01 - Open drain)
	GPIOB->MODER &= ~(0x00000003 << (10 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000002 << (10 * 2)); // Set mode register (0x00 – Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);
}

void buzzer_time_setup(void) {					// pretty much descriptive name, code copied from exercises and changed parameters
	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;

	TIM2->CR1 = (0x0001); // Configure timer 2
	TIM2->ARR = 0xFFFF; // Set reload value to not 63999 //var tidligere: 0xf9ff
	TIM2->PSC = 0x0009; // Set prescale value to 9

	//Kopieret fra 9.1 for at enable the OC3 output
	TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
	TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output

	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
	TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
	TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
	TIM2->CCMR2 |= TIM_OCPreload_Enable;
	TIM2->CCR3 = 0x7FFF; // Set duty cycle to 50 %
}

void buzzer_setup(void){		// sets up both pin and time for buzzer (which could have been written in the same function), but this function is included to make things clear and make debugging easier
	buzzer_pin_setup();			// Only necessary to call the function buzzer_setup() to use driver.
	buzzer_time_setup();
}

void setFreq(uint16_t freq) {					// May be moved to buzzer_sound.c (application interface layer (not hardware abstraction layer)
 uint32_t reload = 64e6 / freq / (0x0009 + 1) - 1;
 TIM2->ARR = reload; // Set auto reload value
 TIM2->CCR3 = reload/2; // Set compare register
 TIM2->EGR |= 0x01;
}
