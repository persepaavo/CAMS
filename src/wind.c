/*
 * wind.c
 *
 * Created: 4.12.2017 13:35:37
 *  Author: e1401187
 */ 

#include "uart.h"
#include "wind.h"

int Wind_measure(void){
	 uint16_t i;
	 uint16_t DatenPaket = 0;
	 cli();

	 for(i=0;i<15;i++)
	 {
		 BitDelay;
		 
		 if(PIND&0X01)
			{
				UART0_Put_Char_blocking('0'); 
				DatenPaket = DatenPaket<<1;	 
			}

		 else
		 {
			 UART0_Put_Char_blocking('1');
			 if(i==0)
			 	{
				 	DatenPaket++;
				 	DatenPaket = DatenPaket<<1;	
			 	}
			
			else
				{
					DatenPaket = DatenPaket<<1;	
					DatenPaket++;
			 	};
			 
		 };
		 
		 
		 
	 };
	 EIMSK &= ~(1<<INT0);
	 UART0_Put_Char_blocking('\n');
	 UART0_Put_Char_blocking('\r');
	 Disable;

	 sei();

return DatenPaket;
 }

int Wind_speed(int data)
 {
	
	int output=0;
	uint16_t bit;
	uint16_t i;

	data = data & 0x1FF;

	for(i=1;i<=9;i++)
		{ 		
			bit= data & 0x01;
			data = data >> 1;	
			output= output << 1;
			if(bit==1)
			output++;
		}
	
	return output; //output must be divided by 10 to get m/s
 }

 int Wind_direction(int data)
 {
	/*
	direction lsb->msb hex
	N 0° 0000 0
	N-NE 22,5 1000 1
	NE 45° 0100 2
	E-NE 67,5° 1100 3
	E 90° 0010 4
	E-SE 112,5° 1010 5
	SE 135° 0110 6
	S-SE 157,5° 1110 7
	S 180° 0001 8
	S-SW 202,5° 1001 9
	SW 225° 0101 A
	W-SW 247,5° 1101 B
	W 270° 0011 C
	W-NW 292,5° 1011 D
	NW 315° 0111 E
	N-NW 337,5° 1111 F
	*/
	int output=0;
	uint16_t bit;
	uint16_t i;

	data = data & 0x1E00;
	data = data >> 9;

	for(i=1;i<=4;i++)
		{
			bit= data & 0x01;
			data = data >> 1;
			output= output << 1;
			if(bit==1)
			output++;
		}

 	return output;
 }

void Wind_start(void)
{

}