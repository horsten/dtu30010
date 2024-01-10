#include <multtimer.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 		// Input/output library for this course
volatile uint8_t flag;
#define TIMERNUM 3 //antal timere

typedef struct { //bygger timer struc
    uint8_t enable;
    uint32_t reset_time;
    uint32_t time_hs;
    void (*callback)(void);
} timerconf;


timerconf timers[TIMERNUM]; //laver et array med timer strukturen til hver timer der kan tilgås ved en indexering


void init_timer(void) {
	RCC->APB2ENR |= RCC_APB2Periph_TIM15; // Enable clock line to timer 15;


	TIM15->CR1 = 0x0001; // Configure timer 15
	TIM15->ARR = 0xf9ff; // Set reload value to 63999
	TIM15->PSC = 0x0009; // Set prescale value to 9


	TIM15->DIER |= 0x0001; // Enable timer 15 interrupts

	NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0x0); // Set interrupt priority
	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn); // Enable interrupt
}

void start_timer(uint8_t timindex, uint32_t time_hs, uint8_t repeating, void (*callback)(void)) { //timer funktionen der modtager hvilken timer der skal bruges ; hvor lang tid den skal vente før callback funtionen eksekveres; funktionen der skal eksekveres
    if (timindex < TIMERNUM) { // sørger for at man ikke kan vælge en timer der ikke er allokeret
        	timers[timindex].enable = repeating ? 1 : 2; //tænder for den valgte timer
        	timers[timindex].reset_time = time_hs;
        	timers[timindex].time_hs = time_hs; //tiden der skal gå før callback funktionen eksekveres i 1/100 sek
        	timers[timindex].callback = callback; //callback funktioen ligges i strukturen til den pågældene funktion
    }
}

void TIM1_BRK_TIM15_IRQHandler(void) { //interrupt handleren der opdaterer flag
		flag = 1;

		TIM15->SR &= ~0x0001; // Clear interrupt bit

	}

void activatecallback(void) { //Denne funktion sørger for callback funktionen forløber rigtigt i forhold til timer15
	while(1){
		if (flag == 1) { //forbinder den til timeren og sørger for at timeren ikke skal arbejde for meget
			for (uint8_t i = 0; i < TIMERNUM; i++) { //gennemgår alle mulige timere
				if (timers[i].enable) { // tjekker hvilke timere der er tændt
					timers[i].time_hs--; //tæller fra tiden givet i start_timer()
					if (timers[i].time_hs == 0) {
						timers[i].callback(); // eksekverer koden til tiden 0
						if (timers[i].enable == 2) {
							timers[i].enable = 0; }//slukker for timer[i]
						timers[i].time_hs = timers[i].reset_time;
					}
            	}
        	}
flag = 0;}
	}
}



void testtimer1(void) {
   printf("I waited 2 seconds for this\n");
}
void testtimer2(void) {
	printf("and 10 seconds for this\n");
}

void testtimer3(void) {
	printf("and 7 seconds for this\n");
}
