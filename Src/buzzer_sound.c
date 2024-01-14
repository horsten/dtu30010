// For at bruge trenger man bare at kalde på buzzer_setup() og setFreq(), med ønsket pitch

#include <buzzer_driver.h>
#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "buzzer_sound.h"
#include "multtimer.h"
#include "ansi.h"
#include "buzzer_driver.h"
volatile uint8_t flag;
/*
 * buzzer_sound.c */

// Everything below is for making a melody:)
void pitch600(void){		// All tones and is called in melody(), setFreq([number]) is the frequency, and it is possible to add more tones as you want
	setFreq(600);
}
void pitch550(void){
	setFreq(550);
}
void pitch500(void){
	setFreq(500);
}
void silent(void){
	setFreq(0);				// frequency "0" is silent.
}

void melody(void){							// is called in main and it is possible to make copies of this with different kind of melodies
	buzzer_setup();							// runs buzzer_driver.
	gotoxy(0,0);
	printf("\nFirst this\n");				// used for debugging (may be removed)
	init_timer();							// sets up the timer, does this make problems for other timers running?
	start_timer(0,25,0,pitch600);			// more description is in multtimer.c where the function is defined, most important is that the time from the start to a pitch function runs is set here, by the second statement in [hs].
	start_timer(1,50,0,pitch550);
	start_timer(2,75,0,pitch500);
	start_timer(3,100,0,silent);
	activatecallback();						// starts the multiple timer operation and is placed in multtimer.c

	while(1){}
}









