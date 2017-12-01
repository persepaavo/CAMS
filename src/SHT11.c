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
// Writes a byte (and return 0 as acknowledge, else 1)
//----------------------------------------------------------------------------------
char send_byte(unsigned char val){
	
//	send_start();
	
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
	_nop_;

	ClockLow;
//	DataOut;
//	DataLow;


	return error;               //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
// reads a byte form the Sensor
// ack = 1 output ACK to sensor
// ack = 0 no ACK to sensor
//----------------------------------------------------------------------------------
char read_byte(unsigned char ack){
		unsigned char i, val = 0;
		DataIn;                   //release DATA-line

		
	/*				
		ClockHigh;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		
		ClockLow;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
	*/
//	/*
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
		}
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		ClockHigh;                    //clk #9 for ack
		_nop_;
		_nop_;
		_nop_;                    //pulswith approx. 5 us
		ClockLow;
		DataIn;                   //release DATA-line
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		_nop_;
		
//	*/
		return val;
}

//----------------------------------------------------------------------------------
// write to register
//----------------------------------------------------------------------------------
void write_register(unsigned char val){
	send_start();
	send_byte(WriteToRegister);
	send_byte(val);
}

//----------------------------------------------------------------------------------
// Changes measuring resolution
// 1 = 8/12 bit Humi/Temp
// 0 = 12/14 bit Humi/Temp (Default!)
//----------------------------------------------------------------------------------
void change_resolution(unsigned char val){
	unsigned char reso;
	if(val == 1){
		reso = 0x01;
	}
	else{
		reso = 0x00;
	}
	write_register(reso);
}

//----------------------------------------------------------------------------------
// Returns temperature in Celsius from sensor
// ex. 15.6C is returned as 156 and so forth
//		int16_t temp_c;
//		temp_c = get_temp();
//
//		c = (signed char)(temp_c % 10);
//		if(c < 0){
//			c=c * -1;
//		}
//		t = (signed char)(temp_c / 10);
//
//		printf("Temp: %d.%dC\n\r", t,c);
//----------------------------------------------------------------------------------
int16_t get_temp(){
	unsigned char t;
	int16_t temp_raw;
	float d1, d2, temp_c;
	d1 = 40.1;
	d2 = 0.01;
	
	send_start();
	t = send_byte(MeasureTemp);
	
	for(t=0; t<320; t++){		// Wait for measurement to be ready (timeout at 320ms)
		_delay_ms(1);
		if((DataRead) == 0){
			break;
		}
	}
	
	temp_raw = ((unsigned char)read_byte(1))<<8;
	temp_raw = temp_raw + (unsigned char)read_byte(0);
	
	temp_c = temp_raw * d2 - d1;
	temp_raw = (int16_t)(temp_c * 10);
	
	return temp_raw;
}

int16_t get_humi(signed char c, signed char t){
	unsigned char i;
	int16_t humi_raw;
	float c1, c2, c3, rhlin, temp_c, t1 ,t2;
	c1 = 2.0468;
	c2 = 0.0367;
	c3 = -0.0000015955;
	t1 = 0.01;
	t2 = 0.00008;
	
	temp_c = (float)c + (float)(t*0.1);
	
	send_start();
	i = send_byte(MeasureHumi);
	
	for(i=0; i<80; i++){		// Wait for measurement to be ready (timeout at 80ms)
		_delay_ms(1);
		if((DataRead) == 0){
			break;
		}
	}
	
	humi_raw = ((unsigned char)read_byte(1))<<8;
	humi_raw = humi_raw + (unsigned char)read_byte(0);
	
	rhlin = c1+c2*humi_raw+c3*(humi_raw*humi_raw);
	
	humi_raw = (float)((temp_c-25)*(t1+t2*humi_raw)+rhlin)*10;	
	return humi_raw;
}
