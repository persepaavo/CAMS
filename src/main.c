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
		
		c = (signed char)(humi_lin % 10);

		t = (signed char)(humi_lin / 10);
				
		printf("  Humidity: %d.%d%%\n\r", t,c);
	
		_delay_ms(2000);
	}	
}