#include <asf.h>
#include "scheduler.h"
#include "SHT11.h"

signed char c=0;
signed char t=0;
	int16_t temp_c;
void task1()
{


	
}


void task2()
{



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
	
	//scheduler_init();
	//sei();
		

	
	while(1) 
	{
	
		change_resolution(0);

	temp_c = get_temp();

	c = (signed char)(temp_c % 10);
	
	if(c < 0)
	{
		c=c * -1;
	}
	
	t = (signed char)(temp_c / 10);
 	if (t!=0)
		PORTB ^= 0x02;


	
	}

	return 0;
}
