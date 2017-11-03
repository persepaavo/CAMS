#include "SHT71_Temp_Humi_sensor.h"

void SendStart(void){

	DATA_LINE_OUT;

	CLOCK_LINE_HIGH;
	_delay_us(1000);
	DATA_LINE_IN;
	DATA_LINE_LOW;
	CLOCK_LINE_LOW;
	CLOCK_LINE_HIGH;
	_delay_us(1000);
	DATA_LINE_HIGH;
	CLOCK_LINE_LOW;
	DATA_LINE_IN;
}

void SendReset (void){
	int k;

	for (k = 0; k < 12; k++) {
		CLOCK_LINE_HIGH;
		_delay_us(1000);
		CLOCK_LINE_LOW;
	}
}

int SendByte(unsigned char byte){
	unsigned char tempbyte;
	int k;

	DATA_LINE_OUT;
	tempbyte = byte;
	for (k = 0x80; k > 0; k /= 2) {

		if (tempbyte & k)
		DATA_LINE_HIGH;
		else
		DATA_LINE_LOW;

		CLOCK_LINE_HIGH;
		_delay_us(1000);
		CLOCK_LINE_LOW;
	}
	DATA_LINE_IN;
	CLOCK_LINE_HIGH;
	CLOCK_LINE_LOW;
	return 1;
}

unsigned char ReadByte(int withack){
	unsigned char tempbyte;
	int k;

	tempbyte = 0;
	DATA_LINE_IN;
	for (k = 0x80; k > 0; k /= 2) {
		CLOCK_LINE_HIGH;
		if (DATA_LINE_READ)
		tempbyte |= k;
		CLOCK_LINE_LOW;
	}
	if (withack) {
		// Acknowledge del byte
		DATA_LINE_OUT;
		DATA_LINE_LOW;
		CLOCK_LINE_HIGH;
		CLOCK_LINE_LOW;
		DATA_LINE_IN;
		} else {
		// Senza acknowledge
		DATA_LINE_OUT;
		DATA_LINE_HIGH;
		CLOCK_LINE_HIGH;
		CLOCK_LINE_LOW;
		DATA_LINE_IN;
	}
	return tempbyte;
}

int ReadTemperature(void){
	unsigned char Lsb, Msb, Chk;
	int sot;
	float temperature_c;

	SendStart();
	if (! SendByte(CMD_READ_TEMP))
	return 0;
	while (DATA_LINE_READ);
	Msb = ReadByte(1);
	Lsb = ReadByte(1);
	Chk = ReadByte(0);
	sot = (Msb << 8) + Lsb;
	temperature_c = (-39.66 + (0.01 * sot))*10;

	return (int)temperature_c;
}

char ReadHumidity(void){
	unsigned char Lsb, Msb, Chk;
	int soh;
	float rel_humidity; 

	SendStart();

	if (! SendByte(CMD_READ_HUM))
	return 0;
	while (DATA_LINE_READ);
	Msb = ReadByte(1);
	Lsb = ReadByte(1);
	Chk = ReadByte(0);

	soh = (Msb << 8) + Lsb;

	rel_humidity = -2.0468 + (0.0367 * soh) + (-1.5955E-6 * soh * soh);

	if (rel_humidity > 99.9)
	rel_humidity = 100.0;
	else if (rel_humidity < 0.1)
	rel_humidity = 0.0;

	return (unsigned char)rel_humidity;
}