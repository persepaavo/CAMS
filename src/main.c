#include "uart.h"
#define F_CPU 8000000L
#include <stdio.h>
#include <util/delay.h>





void main(){
	unsigned int vehicle = 0;
	char blocked = 0;
	UART1_int(9600,8,1,0);


	fdevopen(UART1_Put_Char_blocking,NULL);

	DDRA |= (1<<PINA4);				// PINA4 as output
	DDRA &= ~(1<<PINA5);			// PINA5 as input
	
	PORTA |= (1<<PINA4);			// Test cycle for PINA4
	PORTA &= ~(1<<PINA4);
	_delay_ms(2000);				// wait 2s before doing anything just in case.

	while(1){
		PORTA |= (1<<PINA4);		// Set PINA4 HIGH
		_delay_us(100);				// IR LED pulse width
		PORTA &= ~(1<<PINA4);		// Set PINA4 LOW
		
		_delay_us(200);				// wait 200us for IR sensor to drive PINA5 down, if not blocked
		if(PINA & 0b00100000){		// If PINA5 is high there is something blocking the IR emitter
			blocked = 1;
		}
		else{						// Else IR light reaches sensor
			if(blocked == 1){		// If IR was blocked earlier a vehicle has passed
				vehicle++;
				blocked = 0;
			}
		}
		
		_delay_ms(25);				// wait 25ms before starting a new IR check
		printf("Vehicles: %d\r\n", vehicle);
	}
}