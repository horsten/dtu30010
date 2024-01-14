#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "ansi.h"
#include "fixedpoint_trig.h"
#include "bounceball.h"
#include "joystick.h"
#include "rgb_led.h"

int main(void)
{

// Setup communication with the PC
	uart_init(921600);
	init_term();

#if 0
	test_frame_update();
#elif 1
	bounceball();
#elif 0
	test_joystick_with_rgb_led();
#elif 0
	test_rgb_led();
#elif 0
	test_joystick();
#elif 0
	window(1,1,60,15,0,"Hello World!");
	window(9,4,40,20,0,"Win2: Title Too Long To Fit In Title Bar Of Window");
	window(9,11,60,16,1,"Win3: Alt Style");
	window(29,14,50,26,2,"Notitle");
#elif 0
	color(7,0);
	clrscr();
	test_fp_sincos();
#elif 1
	uint16_t arr[5] = {1, 10, 14, 90, 1};
	uint16_t *ptr1 = &arr[0];
	uint16_t *ptr2 = ptr1 + 3;
	printf ( "%d " , *ptr2);
	printf ( "%d" , ptr2 - ptr1);
#endif

	for(;;);
}
