/*
 * uart.h
 *
 * Created: 20.1.2017 13:53:10
 *  Author: e1401187 & e1401162
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#define XTAL 8000000L

/**
* BAUD = Anna haluttu BAUD nopeus
* SIZE = 5-9 bitti‰
* STOP = 1 tai 2 bitti‰
* PARITY 0=DISABLED, 1=EVEN, 2=ODD
* UART0 menee Robooteille
*/
int UART0_int(long baud, int size, int stop, int parity);

/**
* J‰‰ odottamaan merkki‰.
*/
unsigned char UART0_Get_Char_blocking(void);

/**
* J‰‰ odottamaan merkkijonoa.
* Entterill‰ lopetetaan.
* char *s = Anna taulukko mihin haluat merkkijonon.
*/
int UART0_Get_String_blocking(char *s);

/**
* Tulostaa merkin.
*/
unsigned char UART0_Put_Char_blocking(char c);

/**
* Tulostaa merkkijonon.
*/
int UART0_Put_String_blocking(char *s);
//////////////////////////////////////////////////////
/**
* BAUD = Anna haluttu BAUD nopeus
* SIZE = 5-9 bitti‰
* STOP = 1 tai 2 bitti‰
* PARITY 0=DISABLED, 1=EVEN, 2=ODD
* UART1 menee tietokoneelle
*/
int UART1_int(long baud, int size, int stop, int parity);

/**
* J‰‰ odottamaan merkki‰.
*/
unsigned char UART1_Get_Char_blocking(void);

/**
* J‰‰ odottamaan merkkijonoa.
* Entterill‰ lopetetaan.
* char *s = Anna taulukko mihin haluat merkkijonon.
*/
int UART1_Get_String_blocking(char *s);

/**
* Tulostaa merkin.
*/
unsigned char UART1_Put_Char_blocking(char c);

/**
* Tulostaa merkkijonon.
*/
int UART1_Put_String_blocking(char *s);

#endif /* UART_H_ */