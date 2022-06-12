/*
 * usart.c
 *
 * Created: 17.05.2022 17:47:10
 *  Author: Mariusz
 */ 

#define F_CPU 16000000UL // Clock Speed

#include <avr/io.h>
#include <stdlib.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init() // Inicjalizacja UART
{
	uint16_t ubrr = MYUBRR;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
 }
 
 
 void USART_PutC( char data ) // Wysylamy pojedynczy znak
 {
	 /* Wait for empty transmit buffer */
	 while ( !( UCSR0A & (1<<UDRE0)) );
	 /* Put data into buffer, sends the data */
	 UDR0 = data;
 }
 
 void USART_PutS( char * s) // Wysylamy string, ciag znakow
{
	while( *s ) USART_PutC( *s++ );
}
 
 void USART_PutInt(uint16_t number, uint8_t radix) // Wysylamy liczbe int, radix to w jakim formacie wyswietlamy liczbe 2-dwojkowy 10-dziesiatkowy 16-szesntastkowy
 {
	 char buf[17];
	 itoa(number,buf,radix); //funkcja integer to ascii, jak za male wartosci to zmienic int na log i funkcja ltoa
	 USART_PutS(buf);
 }
 
 
 unsigned char USART_Receive( void )
 {
	 /* Wait for data to be received */
	 while ( !(UCSR0A & (1<<RXC0)) );
	 /* Get and return received data from buffer */
	 return UDR0;
 }
