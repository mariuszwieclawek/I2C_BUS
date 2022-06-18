
#define F_CPU 16000000UL // Clock Speed

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "ring_buffer.h"

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

static RingBuffer USART_RingBuffer_Tx; // UART transmit buffer descriptor
static char RingBufferData_Tx[1024]; // UART transmit buffer memory pool

ISR(USART0_UDRE_vect){
	char tx;
	if(RingBuffer_GetChar(&USART_RingBuffer_Tx,&tx) == true)
		UDR0 = tx; // send char to usart
	else
		UCSR0B &= ~(1<<UDRIE0); // disable the interrupt source
}

void USART_Init(){ // Inicjalizacja UART
	RingBuffer_Init(&USART_RingBuffer_Tx, RingBufferData_Tx, sizeof(RingBufferData_Tx));
	uint16_t ubrr = MYUBRR;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}
 
 
void USART_PutC(char data){ // Wysylamy pojedynczy znak
	cli(); // disable interrupt
	if(RingBuffer_PutChar(&USART_RingBuffer_Tx,data)){ // send char to ring buffer tx
		UCSR0B |= (1<<UDRIE0); // enable interrupt TXE
		sei(); // enable interrupt
	}
	else{
		cli(); // disable interrupt
	}
}
 
void USART_PutS(char * s){ // Wysylamy string, ciag znakow
	while( *s ) USART_PutC( *s++ );
}

void USART_PutC_without_ringbuf(char data){ // Wysylamy pojedynczy znak
	while ( !( UCSR0A & (1<<UDRE0)) ); // Wait for empty transmit buffer
	UDR0 = data; // Put data into buffer, sends the data
}

void USART_PutS_without_ringbuf(char * s){ // Wysylamy string, ciag znakow
	while( *s ) USART_PutC_without_ringbuf( *s++ );
}
 
void USART_PutInt(uint16_t number, uint8_t radix){ // Wysylamy liczbe int, radix to w jakim formacie wyswietlamy liczbe 2-dwojkowy 10-dziesiatkowy 16-szesntastkowy
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
