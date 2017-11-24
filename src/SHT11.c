#include "SHT11.h"


//----------------------------------------------------------------------------------
// Generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
//----------------------------------------------------------------------------------
void send_start(void){
	ClockOut;
	DataOut;
	DataHigh;
	ClockLow;
	_nop_;
	_nop_;
	_nop_;
			
	ClockHigh;
	_nop_;
	DataLow;
	_nop_;
	_nop_;
			
	ClockLow;
	_nop_;
	_nop_;
	_nop_;
			
	ClockHigh;
	_nop_;
	DataHigh;
	_nop_;
	_nop_;
			
	ClockLow;
	_nop_;
	_nop_;
	_nop_;
}

//----------------------------------------------------------------------------------
// Writes a byte and (checks the acknowledge)
//----------------------------------------------------------------------------------
char send_byte(unsigned char val){
	
	send_start();
	
	DataOut;
	DataLow;
	unsigned char i, error = 1;
	for (i = 0x80; i > 0; i /= 2)       //shift bit for masking
	{
		ClockLow;                //clk for SENSI-BUS

		if (i & val){
			DataHigh;           //masking value with i , write to SENSI-BUS
		}
		else{
			DataLow;
		}
		_nop_;
		_nop_;
		_nop_;
		ClockHigh;
		_nop_;
		_nop_;
		_nop_;                //pulswith approx. 5 us
	}

	DataIn;
	ClockLow;
	_nop_;
	_nop_;
	_nop_;
	_nop_;
	_nop_;
	
	error = DataRead;               //check ack (DATA will be pulled down by SHT11)
	ClockHigh;				// Start clock pulse #9
	_nop_;
	_nop_;
	_nop_;
	_nop_;
	
	DataOut;
	DataLow;

	ClockLow;

	return error;               //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
//----------------------------------------------------------------------------------
char read_byte(unsigned char ack){
		unsigned char i, val = 0;
		DataIn;                   //release DATA-line
		for (i = 0x80; i > 0; i /= 2)       //shift bit for masking
		{
			ClockHigh;                //clk for SENSI-BUS
			_nop_;
			if (DataRead){
			val = (val | i);    //read bit
			}
			_nop_;
			_nop_;
			ClockLow;
			_nop_;
			_nop_;
			_nop_;
		}
		if(ack == 1){					//in case of "ack==1" pull down DATA-Line
			DataOut;
			DataLow;
			DataIn;
		}

		ClockHigh;                    //clk #9 for ack
		_nop_;
		_nop_;
		_nop_;                    //pulswith approx. 5 us
		ClockLow;
		DataIn;                   //release DATA-line
		_nop_;
		_nop_;
		_nop_;
		return val;
}