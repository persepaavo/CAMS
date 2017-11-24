#include "uart.h"
#include "SHT11.h"
#include <stdio.h>
#include <util/delay.h>

void main(){
	
	
	char t, c;
	
	UART1_int(9600,8,1,0);
	
	fdevopen(UART1_Put_Char_blocking,NULL);
//	printf("Testi\n\r");
	_delay_ms(2000);
	while (1){

//		send_start();
		t = send_byte(MeasureHumi);
		DataOut;
		DataLow;
		_delay_us(5);
		c = read_byte(t);
		DataOut;
		DataLow;
		printf("Send_byte: %d\n\rRead_byte: %d\n\r", c,t);
		_delay_ms(2000);
	}
	
}