#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define	DataPin				PINA4
#define ClockPin			PINA5
#define ControlDirection	DDRA
#define ControlPort			PORTA
#define DataRead			PINA & 0b00010000					//ignore all but 4th pin


#define ClockOut			ControlDirection |= (1<<ClockPin)	//Set Clock pin as output
#define ClockHigh			ControlPort |= (1<<ClockPin)		//Drive Clock Pin up
#define ClockLow			ControlPort &= ~(1<<ClockPin)		//Drive Clock Pin down

#define DataOut				ControlDirection |= (1<<DataPin)	//Set Data pin as output
#define DataIn				ControlDirection &= ~(1<<DataPin)	//Set Data pin as input
#define DataHigh			ControlPort |= (1<<DataPin)			//Drive Data Pin up
#define DataLow				ControlPort &= ~(1<<DataPin)		//Drive Data Pin down


#define _nop_				_delay_us(2)

#define STATUS_REG_W		0x06	//0000  0110
#define STATUS_REG_R		0x07	//0000  0111
#define MeasureTemp			0x03	//0000  0011
#define MeasureHumi			0x05	//0000  0101
#define RESET				0x1e	//0001  1110


//----------------------------------------------------------------------------------
// Generates a transmission start
//----------------------------------------------------------------------------------
void send_start(void);

//----------------------------------------------------------------------------------
// Writes a byte and checks the acknowledge
//----------------------------------------------------------------------------------
char send_byte(unsigned char val);

//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
//----------------------------------------------------------------------------------
char read_byte(unsigned char ack);
