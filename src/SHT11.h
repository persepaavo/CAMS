#define F_CPU 8000000L
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>



#define	DataPin				PINA0
#define ClockPin			PINA1
#define ControlDirection	DDRA
#define ControlPort			PORTA
#define DataRead			PINA & 0b00000001					//ignore all but 0th pin


#define ClockOut			ControlDirection |= (1<<ClockPin)	//Set Clock pin as output
#define ClockHigh			ControlPort |= (1<<ClockPin)		//Drive Clock Pin up
#define ClockLow			ControlPort &= ~(1<<ClockPin)		//Drive Clock Pin down

#define DataOut				ControlDirection |= (1<<DataPin)	//Set Data pin as output
#define DataIn				ControlDirection &= ~(1<<DataPin)	//Set Data pin as input
#define DataHigh			ControlPort |= (1<<DataPin)			//Drive Data Pin up
#define DataLow				ControlPort &= ~(1<<DataPin)		//Drive Data Pin down


#define _nop_				_delay_ms(2)

#define WriteToRegister		0x06	//0000  0110
#define STATUS_REG_R		0x07	//0000  0111
#define MeasureTemp			0x03	//0000  0011
#define MeasureHumi			0x05	//0000  0101
#define RESET				0x1e	//0001  1110

// if this define is used when writing to register
#define ResolutionBit		0x01	//0000  0001 Forces humidity resolution to 8 bits and temp resolution to 12 bits


//----------------------------------------------------------------------------------
// Generates a transmission start
//----------------------------------------------------------------------------------
void send_start(void);

//----------------------------------------------------------------------------------
// Writes a byte (and return 0 as acknowledge, else 1)
//----------------------------------------------------------------------------------
char send_byte(unsigned char val);

//----------------------------------------------------------------------------------
// reads a byte form the Sensor
// ack = 1 output ACK to sensor
// ack = 0 no ACK to sensor
//----------------------------------------------------------------------------------
char read_byte(unsigned char ack);

//----------------------------------------------------------------------------------
// write to register
//----------------------------------------------------------------------------------
void write_register(unsigned char val);

//----------------------------------------------------------------------------------
// Changes measuring resolution
// 1 = 8/12 bit Humi/Temp
// 0 = 12/14 bit Humi/Temp (Default!)
//----------------------------------------------------------------------------------
void change_resolution(unsigned char val);

//----------------------------------------------------------------------------------
// Returns temperature in Celsius from sensor
// ex. 15.6C is returned as 156 and so forth
// ex. code for main inside SHT11.c
//----------------------------------------------------------------------------------
int16_t get_temp();

int16_t get_humi(signed char c, signed char t);
