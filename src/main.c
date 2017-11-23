#include <asf.h>
#include <util/delay.h>
#include "scheduler.h"

void task1(int* stack)
{
	*stack = (int)0x01;
	PORTB ^= *stack;
	

	
}

void task2(int* stack)
{
	*stack = (int)0x02;	
	PORTB ^= *stack;
	_delay_ms(1000);
	if(*stack == 0x2)
	PORTB ^= 0xf3;

}
void task3(int* stack)
{
	*stack = (int)0x04;

	PORTB ^= *stack;
}

int main (void)
{
	PORTB = 0xff;
	
	DDRB ^= 0xff;
	board_init();
	int *stack = (int*)malloc(sizeof(int)*SCHEDULER_MAX_TASKS );
	
	task_add(2,task1,stack);
	task_add(4,task2,stack);
	task_add(2,task3,stack);
	scheduler_init();
	sei();
	while(1) 
	{

	}

	return 0;
}
