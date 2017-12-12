/*
 * windsensor.c
 *
 * Created: 30.11.2017 13:56:21
 * Author : e1401185
 */ 

#include "uart.h"
#include "wind.h"

uint16_t DatenPaket = 0;
char testdata[50];
uint16_t speed;
uint16_t direction;


int main(void)
{
	EIMSK |= 1 << INT0; //enable int
	EICRA |= 1 << ISC01; //falling edge
	//EICRA |= 1 << ISC20;

	UART0_int(9600,8,1,0);
	UART0_Put_String_blocking("\n\rMORO");
	DataIn;
	EnableOut;
    
	sei();
    
	Enable;

	while (1) 
    {
	
			
    }
}


ISR(INT0_vect) //ulkoinen keskeytys
{
	cli();
	DatenPaket = Wind_measure();
	sprintf(testdata,"testidata: %d\n\r",DatenPaket);
	UART0_Put_String_blocking(testdata);
	
	speed = Wind_speed(DatenPaket);
	direction = Wind_direction(DatenPaket);

	int speedD=speed%10;
	speed=speed/10;
	sprintf(testdata,"speed: %d.%dm/s direction: %X",speed,speedD,direction);
	UART0_Put_String_blocking(testdata);
}

