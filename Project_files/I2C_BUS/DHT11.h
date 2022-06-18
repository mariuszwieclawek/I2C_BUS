
#ifndef DHT11_H_
#define DHT11_H_


#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "usart.h"
#include "LCD1602.h"

#define DHT11_PIN PORTC0		// DHT11 sensor on this pin

typedef struct{
	char I_Humidity;
	char D_Humidity;
	char I_Temperature;
	char D_Temperature;
	char CheckSum;
}DHT11_data;


void DHT_Request(void);				/* Microcontroller send start pulse/request */
void DHT_Response(void);				/* receive response from DHT11 */
uint8_t DHT_Receive_data(void);			/* receive data */
void DHT_Get_Data(DHT11_data* data);
void DHT_print_COM(DHT11_data* data);
void DHT_print_LCD(DHT11_data* data);

#endif /* DHT11_H_ */