/*
 * GPS2.h
 *
 * Created: 23.11.2017 12:52:21
 *  Author: e1401187
 */ 

#ifndef GPS2_H_
#define GPS2_H_

#include <stdbool.h>

void GPS_disable(void);
void GPS_reset(void);
void ParseGPS(char c);
long returnLat(void);
long returnLon(void);

#endif /* GPS2_H_ */