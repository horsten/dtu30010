#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"

int main(void)
{
	// Setup communication with the PC
	uart_init(115200);
	color(7,0);
	clrscr();
	window(2,2,60,15,0,"Hello World!");
	window(10,5,40,20,0,"Win2: Title Too Long To Fit In Title Bar Of Window");
	window(20,12,60,16,1,"Win3: Alt Style");

	for(;;);
}
