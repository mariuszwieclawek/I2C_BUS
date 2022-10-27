#include "DHT11.h"

void DHT_Request(){				/* Microcontroller send start pulse/request */
	DDRC |= (1<<DHT11_PIN);
	PORTC &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(20);			/* wait for 20ms */
	PORTC |= (1<<DHT11_PIN);	/* set to high pin */
}

void DHT_Response(){				/* receive response from DHT11 */
	DDRC &= ~(1<<DHT11_PIN);
	USART_PutS("\nDHTresponseTEST1\n");
	while(PINC & (1<<DHT11_PIN));
	USART_PutS("\nDHTresponseTEST2\n");
	while((PINC & (1<<DHT11_PIN))==0);
	USART_PutS("\nDHTresponseTEST3\n");
	while(PINC & (1<<DHT11_PIN));
	USART_PutS("\nDHTresponseTEST4\n");
}


uint8_t DHT_Receive_data(){			/* receive data, returns 8 bits */ 
	uint8_t c = 0;
	for (int q=0; q<8; q++)
	{
		while((PINC & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PINC & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PINC & (1<<DHT11_PIN));
	}
	return c;
}

void DHT_Get_Data(DHT11_data* data){
	_delay_ms(300);
	DHT_Request();		/* send start pulse */
	USART_PutS("\nDHTTEST\n");
	DHT_Response();		/* receive response */
	USART_PutS("\nDHTTEST\n");
	data->I_Humidity=DHT_Receive_data();	/* store first eight bit in I_HUM */
	data->D_Humidity=DHT_Receive_data();	/* store next eight bit in D_HUM */
	data->I_Temperature=DHT_Receive_data();	/* store next eight bit in I_Temp */
	data->D_Temperature=DHT_Receive_data();	/* store next eight bit in D_Temp */
	data->CheckSum=DHT_Receive_data();/* store next eight bit in CheckSum */
}

void DHT_print_COM(DHT11_data* data){
	char buff[100];
	sprintf(buff, "Humidity = %02d.%02d%%	Temperature = %02d.%02dC\n\r", data->I_Humidity, data->D_Humidity, data->I_Temperature, data->D_Temperature);
	USART_PutS(buff);
}

void DHT_print_LCD(DHT11_data* data){
	char buff[100];
	sprintf(buff, "H=%02d%%", data->I_Humidity);
	LCD1602_goto_xy(11,0);
	LCD1602_send_string(buff);
	
	sprintf(buff, "T=%02dC", data->I_Temperature);
	LCD1602_goto_xy(11,1);
	LCD1602_send_string(buff);
}