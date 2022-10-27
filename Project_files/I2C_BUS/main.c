#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "myTWI.h"
#include "myTWI_second.h"
#include "LCD1602.h"
#include "AM2320.h"
#include "usart.h"
#include "RTC_PCF8563.h"
#include "EEPROM_24AA01.h"
#include "DHT11.h"
#include "TimerCTC.h"

uint16_t one_sec = 0; //seconds counter
uint16_t one_min = 0;

ISR(TIMER1_COMPA_vect) // Interrupt from comparing the contents of the Timer's hardware counter with the OCR1A register
{
	one_sec++;
	if (one_sec == 10)
	{
		one_min = 1;
		one_sec = 0;
	}
}


int main(void){

	TWI_Init();
	TWI2_Init();
	USART_Init();
	TimerCTC_Init();
	LCD1602_Init();
	sei();


	RTC_data data_time;
	RTC_data data_time_from_EEPROM;
	DHT11_data data_weather;
	DHT11_data data_weather_from_EEPROM;
	uint8_t eeprom_dht11_address = 0x10; // eeprom address where will be stored data from dht11
	uint8_t eeprom_rtc_address = 0x50; // eeprom address where will be stored data from rtc
	
	RTC_set_time(12,55,40,15,6,22,4);
	
	bool eeprom_ready = false; // when data read from eeprom is ready
	
	//EEPROM_read_all_data_to_COM();
	while(1){
		RTC_get_time(&data_time);
		RTC_print_COM(&data_time);
		USART_PutS("\nTEST\n");
		RTC_print_LCD(&data_time);
		USART_PutS("\nTEST1\n");
		DHT_Get_Data(&data_weather);
		USART_PutS("\nTEST\n");
		DHT_print_COM(&data_weather);
		USART_PutS("\nTEST\n");
		DHT_print_LCD(&data_weather);
		USART_PutS("\nTEST\n");


		/* Kiedy zosta³y juz zapisane jakieœ wartoœci do pamiêci EEPROM (eeprom_ready = true) */
		if (eeprom_ready == true && one_min==1){
			USART_PutS("\n\r\n\rOdczyt wartosci zapisanych do EEPROM:\n\r");
			_delay_ms(10);
			data_weather_from_EEPROM = EEPROM_read_DHT11(eeprom_dht11_address);
			data_time_from_EEPROM = EEPROM_read_RTC(eeprom_rtc_address);
			RTC_print_COM(&data_time_from_EEPROM);
			RTC_print_LCD(&data_time_from_EEPROM);
			DHT_print_COM(&data_weather_from_EEPROM);
			DHT_print_LCD(&data_weather_from_EEPROM);
			_delay_ms(10);
			USART_PutS("\n\r\n\r\n\r");
			eeprom_ready = false;
		}
		
		/* Zapisujemy wartosci do pamieci EEPROM i informujemy program o przygotowanych danych do odczytu (eeprom_ready = true) */
		if(one_min){
			USART_PutS("\n\r\n\rWykonano zapis wartosci z DHT11 i RTC do pamieci EEPROM:\n\r");
			_delay_ms(10);
			EEPROM_write_DHT11(eeprom_dht11_address,&data_weather);
			EEPROM_write_RTC(eeprom_rtc_address,&data_time);
			RTC_print_COM(&data_time);
			RTC_print_LCD(&data_time);
			DHT_print_COM(&data_weather);
			DHT_print_LCD(&data_weather);
			_delay_ms(10);
			USART_PutS("\n\r\n\r\n\r");
			one_min = 0;
			eeprom_ready = true;
		}
		
		
		_delay_ms(700);
	}
	
	
	
	return 0;
}
