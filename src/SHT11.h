/*
 * SHT11.h
 *
 * Created: 24.11.2017 12:53:35
 *  Author: e1401162
 */ 


#ifndef SHT11_H_
#define SHT11_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <util/delay.h>
#include <avr/io.h>

enum { TEMP, HUMI };

#define	DATA	 PINB6
#define SCK		 PINB5

// #define	SCKHI()  DDRB.1 = 0
// #define	SCKLO()  DDRB.1 = 1

#define ControlDirection	DDRB
#define ControlPort			PORTB
#define DataLineIn			ControlDirection &= ~(1<<DATA) // Set data pin as input
#define DataLineOut			ControlDirection |= (1<<DATA) // Set data pin as output

#define DataLineLow			ControlPort &= ~(1<<DATA) // Drive DATA Pin down
#define DataLineHigh		ControlPort |= (1<<DATA) // Drive DATA Pin up
#define DataLineRead		DATA // Read Data PIN

#define SCKLow			ControlPort &= ~(1<<SCK) // Drive DATA Pin down
#define SCKHigh			ControlPort |= (1<<SCK) // Drive DATA Pin up

ControlDirection &= ~(1<<SCK); // Set data pin as input

#define _nop_()  _delay_us(2)

#define noACK 0
#define ACK   1
//adr  command  r/w
#define STATUS_REG_W 0x06       //000   0011    0
#define STATUS_REG_R 0x07       //000   0011    1
#define MEASURE_TEMP 0x03       //000   0001    1
#define MEASURE_HUMI 0x05       //000   0010    1
#define RESET        0x1e       //000   1111    0

//----------------------------------------------------------------------------------
char s_write_byte(unsigned char val);
//----------------------------------------------------------------------------------
// writes a byte on the Sensibus and checks the acknowledge

//----------------------------------------------------------------------------------
char s_read_byte(unsigned char ack);
//----------------------------------------------------------------------------------
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"

//----------------------------------------------------------------------------------
void s_transstart(void);
//----------------------------------------------------------------------------------
// generates a transmission start
//       _____         ________
// DATA:      |_______|
//           ___     ___
// SCK : ___|   |___|   |______

//----------------------------------------------------------------------------------
void s_connectionreset(void);
//----------------------------------------------------------------------------------
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart
//       _____________________________________________________         ________
// DATA:                                                      |_______|
//          _    _    _    _    _    _    _    _    _        ___     ___
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______

//----------------------------------------------------------------------------------
char s_softreset(void);
//----------------------------------------------------------------------------------
// resets the sensor by a softreset

//----------------------------------------------------------------------------------
char s_read_statusreg(unsigned char *p_value, unsigned char *p_checksum);
//----------------------------------------------------------------------------------
// reads the status register with checksum (8-bit)

//----------------------------------------------------------------------------------
char s_write_statusreg(unsigned char *p_value);
//----------------------------------------------------------------------------------
// writes the status register with checksum (8-bit)

//----------------------------------------------------------------------------------
char s_measure(unsigned char *p_value, unsigned char *p_checksum, unsigned char mode);
//----------------------------------------------------------------------------------
// makes a measurement (humidity/temperature) with checksum

//----------------------------------------------------------------------------------------
void calc_sth11(float *p_humidity, float *p_temperature);
//----------------------------------------------------------------------------------------
// calculates temperature [°C] and humidity [% RH]
// input :  humi [Ticks] (12 bit)
//          temp [Ticks] (14 bit)
// output:  humi [% RH]
//          temp [°C]

//--------------------------------------------------------------------
float calc_dewpoint(float h, float t);
//--------------------------------------------------------------------
// calculates dew point
// input:   humidity [% RH], temperature [°C]
// output:  dew point [°C]


#endif /* SHT11_H_ */