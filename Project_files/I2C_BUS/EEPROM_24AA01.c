
#include "EEPROM_24AA01.h"

void EEPROM_write_by_address(uint8_t address, uint8_t data){
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Write_data(data);
	TWI2_Stop();
}

uint8_t EEPROM_read_from_address(uint8_t address){
	uint8_t value;
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Stop();
	TWI2_Start();
	TWI2_Write_SLA_R(EEPROM_ADDRESS);
	value = TWI2_Read_ACK();
	TWI2_Stop();
	return value;
}

void EEPROM_read_all_data_to_COM(void){
	uint8_t buff[EEPROM_LAST_REGISTER_ADDR+1];
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(EEPROM_FIRST_REGISTER_ADDR);
	TWI2_Stop();
	TWI2_Start();
	TWI2_Write_SLA_R(EEPROM_ADDRESS);
	for(int i = EEPROM_FIRST_REGISTER_ADDR; i < EEPROM_LAST_REGISTER_ADDR; i++){
		buff[i] = TWI2_Read_ACK();
	}
	TWI2_Stop();
	
	char tempbuf[50];
	for(int i = EEPROM_FIRST_REGISTER_ADDR; i < EEPROM_LAST_REGISTER_ADDR; i++){
		sprintf(tempbuf, "EEPROM_ADDRESS = 0x%02x    VALUE = 0x%02x\n\r", i,buff[i]);
		USART_PutS_without_ringbuf(tempbuf);

	}
}


void EEPROM_write_DHT11(uint8_t address, DHT11_data * data){
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Write_data(data->D_Humidity);
	TWI2_Write_data(data->I_Humidity);
	TWI2_Write_data(data->D_Temperature);
	TWI2_Write_data(data->I_Temperature);
	TWI2_Stop();
	_delay_ms(10);
}


DHT11_data EEPROM_read_DHT11(uint8_t address){
	DHT11_data data;
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Stop();
	TWI2_Start();
	TWI2_Write_SLA_R(EEPROM_ADDRESS);
	data.D_Humidity = TWI2_Read_ACK();
	data.I_Humidity = TWI2_Read_ACK();
	data.D_Temperature = TWI2_Read_ACK();
	data.I_Temperature = TWI2_Read_ACK();
	TWI2_Stop();
	_delay_ms(10);
	
	return data;
}


void EEPROM_write_RTC(uint8_t address, RTC_data* data){
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Write_data(data->sec);
	TWI2_Write_data(data->min);
	TWI2_Write_data(data->hour);
	TWI2_Write_data(data->day);
	TWI2_Write_data(data->weekday);
	TWI2_Write_data(data->month);
	TWI2_Write_data(data->year);
	TWI2_Stop();
	_delay_ms(10);
}


RTC_data EEPROM_read_RTC(uint8_t address){
	RTC_data data;
	TWI2_Start();
	TWI2_Write_SLA_W(EEPROM_ADDRESS);
	TWI2_Write_data(address);
	TWI2_Stop();
	TWI2_Start();
	TWI2_Write_SLA_R(EEPROM_ADDRESS);
	data.sec = TWI2_Read_ACK();
	data.min = TWI2_Read_ACK();
	data.hour = TWI2_Read_ACK();
	data.day = TWI2_Read_ACK();
	data.weekday = TWI2_Read_ACK();
	data.month = TWI2_Read_ACK();
	data.year = TWI2_Read_ACK();
	TWI2_Stop();
	_delay_ms(10);
	
	return data;
}