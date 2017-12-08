#include "uart.h"
#include "SHT11.h"
#include <stdio.h>
#include <util/delay.h>


void main(){
	
	
	signed char t;
	signed char c;
	signed char m;
	signed char k;

	int irCounter;
	
	UART1_int(9600,8,1,0);
	
	fdevopen(UART1_Put_Char_blocking,NULL);

	unsigned int vehicle = 0;
	char blocked = 0;

	DDRA |= (1<<PINA4);				// PINA4 as output
	DDRA &= ~(1<<PINA5);			// PINA5 as input
	
	PORTA |= (1<<PINA4);			// Test cycle for PINA4
	PORTA &= ~(1<<PINA4);
	_delay_ms(2000);				// wait 2s before doing anything just in case.



	while (1){



	//////////////////////////////////////////////
	/////// IR                             ///////
	//////////////////////////////////////////////

	for(irCounter=0;irCounter<200;irCounter++){ // demo wait 2400 for 2 mins
		
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
		
		_delay_ms(50);				// wait 25ms before starting a new IR check
		
	}

	//////////////////////////////////////////////
	/////// SHT11                          ///////
	//////////////////////////////////////////////

	change_resolution(0);

/*
		send_start();
		t = send_byte(MeasureHumi);
	
		for(t=0; t < 20; t++){		// Wait for measurement to be ready (timeout at 20ms)
			_delay_ms(1);
			if((DataRead) == 0){
				break;
			}
		}
				
		c = (unsigned char)read_byte(1);
*/	


	int16_t temp_c;
	temp_c = get_temp();
		
	c = (signed char)(temp_c % 10);
	if(c < 0){
		c=c * -1;
	}
	t = (signed char)(temp_c / 10);
	
	printf("Temp: %d.%dC", t,c);
	

	int16_t humi_lin;
	humi_lin = get_humi(c, t);
		
	if(humi_lin > 990){
		m=0;
		k=100;
	}
	else{
		m = (signed char)(humi_lin % 10);
		k = (signed char)(humi_lin / 10);	
	}
				
	printf("  Humidity: %d.%d%%\n\r", k,m);


	printf("Vehicles: %d\r\n", vehicle);
	_delay_ms(2000);
		




	}
}
