#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "fixedpoint_trig.h"

int main(void)
{

// Setup communication with the PC
	uart_init(460800);
	color(7,0);
	clrscr();

#if 0
	window(2,2,60,15,0,"Hello World!");
	window(10,5,40,20,0,"Win2: Title Too Long To Fit In Title Bar Of Window");
	window(20,12,60,16,1,"Win3: Alt Style");
#endif

	color(7,0);
	clrscr();
	test_fp_sincos();

	for(;;);
}
