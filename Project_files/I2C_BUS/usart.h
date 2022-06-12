/*
 * usart.h
 *
 * Created: 17.05.2022 17:47:38
 *  Author: Mariusz
 */ 


#ifndef USART_H_
#define USART_H_

 // Inicjalizacja UART
void USART_Init();

 // Wysylamy pojedynczy znak
void USART_PutC( char data );

// Wysylamy string, ciag znakow
void USART_PutS( char * s);

// Wysylamy liczbe int, radix to w jakim formacie wyswietlamy liczbe 2-dwojkowy 10-dziesiatkowy 16-szesntastkowy
void USART_PutInt(uint16_t number, uint8_t radix);

// Odczytujemy pojedynczy znak
unsigned char USART_Receive( void );

#endif /* USART_H_ */