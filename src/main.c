/**
 * \file
 *
 * \brief Empty user application template
 *	hi
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <util/delay.h>
#include "scheduler.h"

void task1(void)
{
	PORTB ^= 0x01;
	//_delay_ms(1000);
}

void task2(void)
{
	PORTB ^= 0x05;
}

int main (void)
{
	PORTB =0xff;
	/* Insert  system clock initialization code here (sysclk_init()). */

	
	DDRB ^= 0xff;

	board_init();
	task_add(0,1,task1);
	task_add(0,1,task2);
	scheduler_init();
	sei();
	while(1) 
	{

	}
	/* Insert application code here, after the board has been initialized. */

	return 0;
}
