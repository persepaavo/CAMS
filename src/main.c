#include "uart.h"
#define F_CPU 8000000L
#include <stdio.h>
#include <util/delay.h>


int valoportti;

ISR(INT5_vect)
{
	valoportti++;
	printf("Autoja: %d\r\n", valoportti);
	_delay_ms(300);
}

void main(){
	
	UART1_int(9600,8,1,0);
	EIMSK |= (1 << INT5);
	EICRB |= (1 << ISC50);
	EICRB |= (1 << ISC51);

	fdevopen(UART1_Put_Char_blocking,NULL);
	sei();

	DDRA |= (1<<PINA4);
	
	PORTA |= (1<<PINA4);
	PORTA &= ~(1<<PINA4);
	_delay_ms(2000);
//		printf("Testi\n\r");
	while (1){		
		PORTA |= (1<<PINA4);
		_delay_us(167);
		PORTA &= ~(1<<PINA4);
		_delay_us(167);
	}
}