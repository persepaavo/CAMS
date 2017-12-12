/*
 * wind.h
 *
 * Created: 4.12.2017 13:35:57
 *  Author: e1401187
 */ 

#ifndef WIND_H_
#define WIND_H_

#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define DataPin				PIND0
#define EnablePin			PIND1
#define ControlDirection	DDRD
#define ControlPort			PORTD

#define DataIn				ControlDirection &= ~(1<<DataPin)
#define EnableOut			ControlDirection |= (1<<EnablePin)
#define Enable				ControlPort &= ~(1<<EnablePin) //Set enable low
#define Disable				ControlPort |= (1<<EnablePin) //Set enable high
#define StartDelay			_delay_us(610);
#define BitDelay			_delay_us(1220);
//#define BitDelay2			_delay_us()
//#define BitDelay3			_delay_us()


int Wind_measure(void);
int Wind_speed(int data);
int Wind_direction(int data);
void Wind_start(void);


#endif /* WIND_H_ */