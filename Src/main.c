#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "fixedpoint_trig.h"
#include "bounceball.h"
#include "joystick.h"

int main(void)
{

// Setup communication with the PC
	uart_init(460800);
	init_term();

#if 0
	bounceball();
#elif 1
	test_joystick();
#elif 0
	window(1,1,60,15,0,"Hello World!");
	window(9,4,40,20,0,"Win2: Title Too Long To Fit In Title Bar Of Window");
	window(9,11,60,16,1,"Win3: Alt Style");
	window(29,14,50,26,2,"Notitle");
#elif 1
	color(7,0);
	clrscr();
	test_fp_sincos();
#endif

	for(;;);
}
