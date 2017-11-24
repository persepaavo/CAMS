/*
 * SHT11.c
 *
 * Created: 24.11.2017 12:53:21
 *  Author: e1401162
 */ 
#include "SHT11.h"

//----------------------------------------------------------------------------------
char s_write_byte(unsigned char val)
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge
{
	unsigned char i, error = 0;
	DataLineOut;
	for (i = 0x80; i > 0; i /= 2)       //shift bit for masking
	{
		if (i & val){
			DataLineHigh;           //masking value with i , write to SENSI-BUS
		}
		else{
			DataLineLow;
		}
		SCKHigh;                //clk for SENSI-BUS
		_nop_();
		_nop_();
		_nop_();                //pulswith approx. 5 us
		SCKLow;
	}
	DataLineIn;						//release DATA-line
	SCKHigh;                    //clk #9 for ack
	error = DATA;               //check ack (DATA will be pulled down by SHT11)
	SCKLow;
	return error;               //error=1 in case of no acknowledge
}

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack)
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"
{
	unsigned char i, val = 0;
	DataLineIn();                   //release DATA-line
	for (i = 0x80; i > 0; i /= 2)       //shift bit for masking
	{
		SCKHigh;                //clk for SENSI-BUS
		if (DATA)
		val = (val | i);    //read bit
		SCKLow;
	}
	if(ack == 1){
	DataLineOut;
	DataLineLow;                //in case of "ack==1" pull down DATA-Line
	}
	DataLineOut;
	SCKHigh;                    //clk #9 for ack
	_nop_();
	_nop_();
	_nop_();                    //pulswith approx. 5 us
	SCKLow;
	DataLineIn();                   //release DATA-line
	return val;
}

//----------------------------------------------------------------------------------
void s_transstart(void)
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______
{
	DataLineOut;
	DataLineHigh;
	SCKLow;                    //Initial state
	_nop_();
	SCKHigh;
	_nop_();
	DataLineLow;
	_nop_();
	SCKLow;
	_nop_();
	_nop_();
	_nop_();
	SCKHigh;
	_nop_();
	DataLineHigh;
	_nop_();
	SCKLow;
}

//----------------------------------------------------------------------------------
void s_connectionreset(void)
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
{
	unsigned char i;
	DataLineOut;
	DataLineHigh;
	SCKLow;                    //Initial state
	for (i = 0; i < 9; i++)     //9 SCK cycles
	{
		SCKHigh;
		SCKLow;
	}
	s_transstart();             //transmission start
}

//----------------------------------------------------------------------------------
char s_softreset(void)
//----------------------------------------------------------------------------------
// resets the sensor by a softreset
{
	unsigned char error = 0;
	s_connectionreset();        //reset communication
	error += s_write_byte(RESET);       //send RESET-command to sensor
	return error;               //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum)
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)
{
	unsigned char error = 0;
	s_transstart();             //transmission start
	error = s_write_byte(STATUS_REG_R); //send command to sensor
	*p_value = s_read_byte(ACK);        //read status register (8-bit)
	*p_checksum = s_read_byte(noACK);   //read checksum (8-bit)
	return error;               //error=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value)
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)
{
	unsigned char error = 0;
	s_transstart();             //transmission start
	error += s_write_byte(STATUS_REG_W);        //send command to sensor
	error += s_write_byte(*p_value);    //send value of status register
	return error;               //error>=1 in case of no response form the sensor
}

//----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode)
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum
{
	unsigned error = 0;
	unsigned int i;

	s_transstart();             //transmission start
	switch (mode) {             //send command to sensor
		case TEMP:
		error += s_write_byte(MEASURE_TEMP);
		break;
		case HUMI:
		error += s_write_byte(MEASURE_HUMI);
		break;
		default:
		break;
	}
	for (i = 0; i < 2000; i++) {
		if (DATA == 0)
		break;              //wait until sensor has finished the measurement
		_delay_ms(1);
	}
	if (DATA)
	error += 1;             // or timeout (~2 sec.) is reached
	*(p_value) = s_read_byte(ACK);      //read the first byte (MSB)
	*(p_value + 1) = s_read_byte(ACK);  //read the second byte (LSB)
	*p_checksum = s_read_byte(noACK);   //read checksum
	return error;
}

//----------------------------------------------------------------------------------------
void calc_sth11(float *p_humidity, float *p_temperature)
//----------------------------------------------------------------------------------------
// calculates temperature [°C] and humidity [% RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [% RH]
//          temp [°C]
{
	const float C1 = -4.0;      // for 12 Bit
	const float C2 = +0.0405;   // for 12 Bit
	const float C3 = -0.0000028;        // for 12 Bit
	const float T1 = +0.01;     // for 14 Bit @ 5V
	const float T2 = +0.00008;  // for 14 Bit @ 5V

	float rh = *p_humidity;     // rh:      Humidity [Ticks] 12 Bit
	float t = *p_temperature;   // t:       Temperature [Ticks] 14 Bit
	float rh_lin;               // rh_lin:  Humidity linear
	float rh_true;              // rh_true: Temperature compensated humidity
	float t_C;                  // t_C   :  Temperature [°C]

	t_C = t * 0.01 - 40;        //calc. temperature from ticks to [°C]
	rh_lin = C3 * rh * rh + C2 * rh + C1;       //calc. humidity from ticks to [% RH]
	rh_true = (t_C - 25) * (T1 + T2 * rh) + rh_lin;     //calc. temperature compensated humidity [% RH]
	if (rh_true > 100)
	rh_true = 100;          //cut if the value is outside of
	if (rh_true < 0.1)
	rh_true = 0.1;          //the physical possible range

	*p_temperature = t_C;       //return temperature [°C]
	*p_humidity = rh_true;      //return humidity[% RH]
}

//--------------------------------------------------------------------
float calc_dewpoint(float h, float t)
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [% RH], temperature [°C]
// output:  dew point [°C]
{
	float logEx, dew_point;
	logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
	dew_point = (logEx - 0.66077) * 237.3 / (0.66077 + 7.5 - logEx);
	return dew_point;
}