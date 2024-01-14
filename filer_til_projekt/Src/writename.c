
#include <stdint.h>
#include <stdio.h>
#include <writename.h>
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h"
	char direction;
	char test;
void keyboardex(){
		char theold = direction;
		while(1){
		direction = uart_get_char();
		if (theold != direction){
			printf("%c",direction);
		}
		}
}

char testlives(){
		while(1){

			test = uart_get_char();
			return test;
			}
}

