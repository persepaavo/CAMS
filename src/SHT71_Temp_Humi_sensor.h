/*
 * SHT71_Temp_Humi_sensor.h
 *
 * Created: 3.11.2017 13:15:04
 *  Author: e1401162
 */ 


#ifndef SHT71_TEMP_HUMI_SENSOR_H_
#define SHT71_TEMP_HUMI_SENSOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <util/delay.h>
#include <avr/io.h>

#ifdef __CRIS__
#include "linux/gpio_syscalls.h"  // gpioset commands needs to be changed to avr equivalents
#endif

// -----------------------------------------------------------------------
// SHT71 sensor define
// -----------------------------------------------------------------------

#define CLOCK_BIT         PINA7    // PINA7
#define DATA_BIT          PINA6    // PINA6

#define CLOCK_MSK         (1 << CLOCK_BIT)
#define DATA_MSK          (1 << DATA_BIT)

/*
// Macro to set the data line direction
#define DATA_LINE_IN      gpiosetdir(PORTG,DIRIN,DATA_MSK)
#define DATA_LINE_OUT     gpiosetdir(PORTG,DIROUT,DATA_MSK)

#define DATA_LINE_LOW     gpiosetdir(PORTG,DIROUT,DATA_MSK);gpioclearbits(PORTG,DATA_MSK)
#define DATA_LINE_HIGH    gpiosetdir(PORTG,DIRIN,DATA_MSK)
#define DATA_LINE_READ    gpiogetbits(PORTG,DATA_MSK)?(1):(0)

#define CLOCK_LINE_LOW    gpioclearbits(PORTG,CLOCK_MSK)
#define CLOCK_LINE_HIGH   gpiosetbits(PORTG,CLOCK_MSK)

*/

#define ControlDirection	DDRA
#define ControlPort			PORTA
#define DATA_LINE_IN		ControlDirection &= ~(1<<DATA_BIT) // Set data pin as input
#define DATA_LINE_OUT		ControlDirection |= (1<<DATA_BIT) // Set data pin as output

#define DATA_LINE_LOW		DATA_LINE_OUT, ControlPort &= ~(1<<DATA_BIT) // Set data pin as output and drive it down
#define DATA_LINE_HIGH		DATA_LINE_OUT, ControlPort |= (1<<DATA_BIT) // Set data pin as output and drive it up
#define DATA_LINE_READ		DATA_LINE_IN, DATA_BIT // Set data pin as input and read it

#define CLOCK_LINE_LOW		DATA_LINE_OUT, ControlPort &= ~(1<<CLOCK_BIT) // Set clock pin as output and drive it down
#define CLOCK_LINE_HIGH		DATA_LINE_OUT, ControlPort |= (1<<DATA_BIT) // Set clock pin as output and drive it up


#define CMD_READ_TEMP     0x03
#define CMD_READ_HUM      0x05

// -----------------------------------------------------------------------
// Send the start sequence
// -----------------------------------------------------------------------
void SendStart(void);

// -----------------------------------------------------------------------
// Sensor reset
// -----------------------------------------------------------------------
void SendReset (void);

// -----------------------------------------------------------------------
// Send a request to the sensor
// Possible arguments
// CMD_READ_TEMP to receive temperature
// CMD_READ_HUM to receive humidity
// -----------------------------------------------------------------------
int SendByte(unsigned char byte);

// -----------------------------------------------------------------------
// Read a byte from the sensor
// withack
//   1 = send the ACK
//   0 = don't send the ACK
// -----------------------------------------------------------------------
unsigned char ReadByte(int withack);

// ----------------------
// Read the temperature
// ----------------------
int ReadTemperature(void);

// ------------------
// Read the humidity
// Returns char value 0-100
// ------------------
char ReadHumidity(void);

#endif /* SHT71_TEMP_HUMI_SENSOR_H_ */