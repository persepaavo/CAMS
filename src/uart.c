/*
 * uart.c
 *
 * Created: 20.1.2017 14:44:33
 *  Author: e1401187 & e1401162
 */ 
#include "uart.h"
int UART0_int(long baud, int size, int stop, int parity){
	UBRR0H = 0x00; // write high register first
	UBRR0L = ((XTAL/16)/baud)-1;
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); // 0001 1000
	// Bit 4 – RXEN: Receiver Enable, vastaanotto sallittu
	// Bit 3 – TXEN: Transmitter Enable, lähetys sallittu
	UCSR0B |= (1<<RXCIE0); // rx interrupt enable, 1xx1 1xxx
	// sarjavastaanottokeskeytys sallittu

	if(stop==1)
	UCSR0C |=(0<<USBS0);
	if(stop==2)
	UCSR0C |=(1<<USBS0);

	if(size==5)
	UCSR0C |= (0<<UCSZ01)|(0<<UCSZ00);
	if(size==6)
	UCSR0C |= (0<<UCSZ01)|(1<<UCSZ00);
	if(size==7)
	UCSR0C |= (1<<UCSZ01)|(0<<UCSZ00);
	if(size==8)
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	if(size==9){
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B |= (1<<UCSZ02);
	}

	if(parity==0)	//disabled
	UCSR0C |= (0<<UPM01)|(0<<UPM01);
	if(parity==1)	//enabled,EVEN
	UCSR0C |= (1<<UPM01)|(0<<UPM01);
	if(parity==2)	//enabled,ODD
	UCSR0C |= (1<<UPM01)|(1<<UPM01);
	return 0;	
}

unsigned char UART0_Put_Char_blocking(char c){
#if WIN
	if(c=='\n')
		UART0_Put_Char_blocking('\r');
#endif

	while (!( UCSR0A & (1<<UDRE0))); // Odotetaan merkki on lähetetty
	UDR0=c;
	return 0;
}

int UART0_Put_String_blocking(char *s){
	int i=0;
	while(*(s+i) != '\0'){
		UART0_Put_Char_blocking(*(s+i));
		i++;
	}
	return 0;
}

unsigned char UART0_Get_Char_blocking(){
	while (!(bit_is_set(UCSR0A, RXC))); // odotetaan kunnes saadaan joku merkki
	return UDR0;
}

int UART0_Get_String_blocking(char *s){
	char c;
	int i=0;
	while (c != 0x0D){ //Jos joku muu merkki kuin entteri
		c=UART0_Get_Char_blocking();
		if(c != 0x0D){ // Laitetaan taulukkoon jos jokin muu kuin enter
			*(s+i)=c;
			i++;
		}
		*(s+i)='\0';
	}
	return 0;
}

int UART1_int(long baud, int size, int stop, int parity){
	UBRR1H = 0x00; // write high register first
	UBRR1L = ((XTAL/16)/baud)-1;
	UCSR1B |= (1<<RXEN1)|(1<<TXEN1); // 0001 1000
	// Bit 4 – RXEN: Receiver Enable, vastaanotto sallittu
	// Bit 3 – TXEN: Transmitter Enable, lähetys sallittu
	UCSR1B |= 1<<RXCIE1; // rx interrupt enable, 1xx1 1xxx
	// sarjavastaanottokeskeytys sallittu

	if(stop==1)
	UCSR1C |=(0<<USBS1);
	if(stop==2)
	UCSR1C |=(1<<USBS1);

	if(size==5)
	UCSR1C |= (0<<UCSZ11)|(0<<UCSZ10);
	if(size==6)
	UCSR1C |= (0<<UCSZ11)|(1<<UCSZ10);
	if(size==7)
	UCSR1C |= (1<<UCSZ11)|(0<<UCSZ10);
	if(size==8)
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
	if(size==9){
		UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
		UCSR1B |= (1<<UCSZ12);
	}

	if(parity==0)	//disabled
	UCSR1C |= (0<<UPM11)|(0<<UPM11);
	if(parity==1)	//enabled,EVEN
	UCSR1C |= (1<<UPM11)|(0<<UPM11);
	if(parity==2)	//enabled,ODD
	UCSR1C |= (1<<UPM11)|(1<<UPM11);
	return 0;
}

unsigned char UART1_Put_Char_blocking(char c){
	#if WIN
	if(c=='\n')
	UART1_Put_Char_blocking('\r');
	#endif

	while (!( UCSR1A & (1<<UDRE1)));
	UDR1=c;
	return 0;
}

int UART1_Put_String_blocking(char *s){
	int i=0;
	while(*(s+i) != '\0'){ // Käydään merkkitaulukko läpi
		UART1_Put_Char_blocking(*(s+i));
		i++;
	}
	return 0;
}

unsigned char UART1_Get_Char_blocking(){
	while (!(bit_is_set(UCSR1A, RXC))); // odotetaan kunnes saadaan joku merkki
	return UDR1;
}

int UART1_Get_String_blocking(char *s){
	char c;
	int i=0;
	while (c != 0x0D){ //Jos joku muu merkki kuin entteri
		c=UART1_Get_Char_blocking();
		if(c != 0x0D){ // Laitetaan taulukkoon jos jokin muu kuin enter
			*(s+i)=c;
			i++;
		}
		*(s+i)='\0';
	}
	return 0;
}