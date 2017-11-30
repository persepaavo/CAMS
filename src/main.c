#include "uart.h"
#include "SHT11.h"
#include <stdio.h>
#include <util/delay.h>


void main(){
	
	
	signed char t;
	signed char c;
	
	UART1_int(9600,8,1,0);
	
	fdevopen(UART1_Put_Char_blocking,NULL);
//	printf("Testi\n\r");
	_delay_ms(2000);
	while (1){
		change_resolution(1);
/*
		send_start();
		t = send_byte(MeasureTemp);
	
		for(t=0; t<80; t++){		// Wait for measurement to be ready (timeout at 80ms)
			_delay_ms(1);
			if((DataRead) == 0){
				break;
			}
		}
				
		c = (unsigned char)read_byte(1);
		d = (unsigned char)read_byte(0);
*/		
		int16_t temp_c;
		temp_c = get_temp();
		
		c = (signed char)(temp_c % 10);
		if(c < 0){
			c=c * -1;
		}
		t = (signed char)(temp_c / 10);
		
		printf("Temp: %d.%dC\n\r", t,c);
		_delay_ms(2000);
	}	
}