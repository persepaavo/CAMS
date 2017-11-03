/*
 * SHT71_Temp_Humi_sensor.c
 *
 * Created: 26.10.2017 17.51.09
 *  Author: henri
 */ 

// Example of user space C program to read a humidity and temperature
// sensor Sensirion SHT71 (http://www.acmesystems.it/?id=89)

#include "SHT71_Temp_Humi_sensor.h"
#include "uart.h"

int main(void){

	char humidity;
	int temperature;
	char tempH;
	char tempL;

	UART1_int(9600,8,1,0);
	UART1_Put_String_blocking("Aloitetaan\n\r"); // Sarjaliikenteen testauslähetys

	
	humidity = ReadHumidity();

	temperature = ReadTemperature();
	tempH = (char)(temperature/10);
	tempL = (char)(temperature-(tempH*10));

	UART1_Put_String_blocking("Temperature: ");
	UART1_Put_Char_blocking(tempH);
	UART1_Put_String_blocking(".");
	UART1_Put_Char_blocking(tempL);

	UART1_Put_String_blocking("Humidity: ");
	UART1_Put_Char_blocking(humidity);
	UART1_Put_String_blocking("%/n/r");
	
}