/*
 * main.c
 *
 * Created: 7.9.2017 11:32:00
 * Author : e1401187
 */ 

#include <avr/io.h>
#include <assert.h>
#include "uart.h"
#include "GPS2.h"

#include <stdbool.h>
#include <stdio.h>

char string[100];
char test[100];
int t=0;
long testLat;
long testLon;
float rejf1,rejf2;

float kalle =0.5;




ISR(USART0_RX_vect)
{
	ParseGPS(UDR0);
//	UART1_Put_Char_blocking(UDR0);
}


int main(void){
	
	UART0_int(9600,8,1,0);
	UART1_int(9600,8,1,0);
	sei(); // global interrut ON

//	GPS_reset();
	GPS_disable();

    while (1)
		{	
			cli();
			testLat=returnLat();
			testLon=returnLon();
			sei();
			rejf1=(float)testLat/600000; // Calculate latitude
			rejf2=(float)testLon/600000; // Calculate longitude
			sprintf(string,"Latitude: %f Longitude: %f",rejf1,rejf2);
			UART1_Put_String_blocking(string);
			UART1_Put_String_blocking("\n\r");
		}
}
