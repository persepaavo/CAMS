#include <asf.h>
#include <util/delay.h>
#include "scheduler.h"

void task1()
{

	PORTB ^= 0x01;	
}

void task2()
{

	PORTB ^= 0x02;

}
void task3()
{


	PORTB ^= 0x04;
}

int main (void)
{
	PORTB = 0xff;
	
	DDRB ^= 0xff;
	board_init();

	
	task_add(2,0, 0, 0,task1);
	task_add(4,0,6,0,task2);
	task_add(61, 61, 3, 0,task3);
	scheduler_init();
	sei();
	while(1) 
	{

	}

	return 0;
}
