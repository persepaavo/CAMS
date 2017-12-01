/*
 * GPS2.c
 *
 * Created: 23.11.2017 12:34:59
 *  Author: e1401187
 */
#include "GPS2.h"
#include "uart.h"

char fmt[]="$GPRMC,dddtdd.ddm,A,eeae.eeee,l,eeeae.eeee,o,djdk,djdc,dddy??,,,?*??";
int state = 0;
unsigned int temp;
long ltmp;

// GPS variables
volatile unsigned int Time, Msecs, Knots, Course, Date;
volatile long Lat, Long;
volatile bool Fix;

void GPS_disable()
 {
	 char message[]="$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
	 UART0_Put_String_blocking(message);
 }

void GPS_reset()
 {
	 char message[]="$PMTK104*37\r\n";
	 UART0_Put_String_blocking(message);
 }

long returnLat()
{
return Lat;
}

long returnLon()
{
	return Long;
}

void ParseGPS (char c) {
	if (c == '$') { state = 0; temp = 0; }
	char mode = fmt[state++];
	// If received character matches format string, or format is '?' - return
	if ((mode == c) || (mode == '?')) return;
	// d=decimal digit
	char d = c - '0';
	if (mode == 'd') temp = temp*10 + d;
	// e=long decimal digit
	else if (mode == 'e') ltmp = (ltmp<<3) + (ltmp<<1) + d; // ltmp = ltmp*10 + d;
	// a=angular measure
	else if (mode == 'a') ltmp = (ltmp<<2) + (ltmp<<1) + d; // ltmp = ltmp*6 + d;
	// t=Time - hhmm
	else if (mode == 't') { Time = temp*10 + d; temp = 0; }
	// m=Millisecs
	else if (mode == 'm') { Msecs = temp*10 + d; ltmp=0; }
	// l=Latitude - in minutes*1000
	else if (mode == 'l') { if (c == 'N') Lat = ltmp; else Lat = -ltmp; ltmp = 0; }
	// o=Longitude - in minutes*1000
	else if (mode == 'o') { if (c == 'E') Long = ltmp; else Long = -ltmp; temp = 0; }
	// j/k=Speed - in knots*100
	else if (mode == 'j') { if (c != '.') { temp = temp*10 + d; state--; } }
	else if (mode == 'k') { Knots = temp*10 + d; temp = 0; }
	// c=Course (Track) - in degrees*100
	else if (mode == 'c') { Course = temp*10 + d; temp = 0; }
	// y=Date - ddmm
	else if (mode == 'y') { Date = temp*10 + d ; Fix = 1; }
	else state = 0;
}