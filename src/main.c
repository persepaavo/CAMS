#include "uart.h"
#include "SHT11.h"
#include <stdio.h>


#include <util/delay.h>
// Declare your global variables here
typedef union {
	unsigned int i;
	float f;
} value;

//----------------------------------------------------------------------------------
void main()
//----------------------------------------------------------------------------------
// sample program that shows how to use SHT11 functions
// 1. connection reset
// 2. measure humidity [ticks](12 bit) and temperature [ticks](14 bit)
// 3. calculate humidity [% RH] and temperature [°C]
// 4. calculate dew point [°C]
// 5. print temperature, humidity, dew point
{
	value humi_val, temp_val;
	float dew_point;
	unsigned char error, checksum;
	unsigned int i;

	UART1_int(9600,8,1,0);
	fdevopen(UART1_Put_Char_blocking,NULL);
	s_connectionreset();
	while (1) {
		error = 0;
		error += s_measure((unsigned char *) &humi_val.i, &checksum, HUMI);     //measure humidity
		error += s_measure((unsigned char *) &temp_val.i, &checksum, TEMP);     //measure temperature
		if (error != 0)
		s_connectionreset();        //in case of an error: connection reset
		else {
			humi_val.f = (float) humi_val.i;    //converts integer to float
			temp_val.f = (float) temp_val.i;    //converts integer to float
			calc_sth11(&humi_val.f, &temp_val.f);       //calculate humidity, temperature
			dew_point = calc_dewpoint(humi_val.f, temp_val.f);  //calculate dew point
			printf("temp:%05.1fC humi:%05.1f%% dew point:%05.1fC\n", temp_val.f, humi_val.f, dew_point);
		}
		//----------wait approx. 0.8s to avoid heating up SHTxx------------------------------
		_delay_ms(800);          //(be sure that the compiler doesn't eliminate this line!)
		//-----------------------------------------------------------------------------------
	}
	
}