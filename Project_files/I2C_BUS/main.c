#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#include "myTWI.h"
#include "LCD1602.h"
#include "AM2320.h"
#include "usart.h"
#include "RTC_PCF8563.h"
#include "EEPROM_24AA01.h"

int main(void){
	
	
	/*
	// EEPROM
	USART_Init();
	TWI_Init();

	EEPROM_write_by_address(0x00,0xa4);
	EEPROM_write_by_address(0x07,0x53);
	EEPROM_write_by_address(0x72,0x39);
	EEPROM_write_by_address(0x46,0x16);
	
	EEPROM_read_all_data();
	*/
	
	
	TWI_Init();
	USART_Init();
	//TWI_Ping_Slave();
	LCD1602_Init();
	
	sensor.high_temperature = 25;
	sensor.low_temperature = 15;
	sensor.high_humidity = 56;
	sensor.low_humidity = 76;
	
	struct tm data_time;
	
	RTC_set_time(17,55,40,12,6,22,7);

	while(1){
		RTC_get_time(&data_time);
		RTC_print_COM(&data_time);
		RTC_print_LCD(&data_time);
		AM2320_print_COM();
		AM2320_print_LCD();
		
		_delay_ms(1000);
	}
	
	
	/*
	// RTC CLOCK
	USART_Init();
	TWI_Init();
	USART_PutS("1\n\r");
	//TWI_Ping_Slave();
	
	struct tm data_time;
	
	set_time(17,5,10,8,6,22,3);
	
	while(1){
		get_time(&data_time);
		
		print_time_COM(&data_time);
		
		_delay_ms(1000);
	}
	*/
	
	
	return 0;
}
