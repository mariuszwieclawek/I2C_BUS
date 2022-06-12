
#include "EEPROM_24AA01.h"

void EEPROM_write_by_address(uint8_t address, uint8_t data){
	TWI_Start();
	TWI_Write_SLA_W(EEPROM_ADDRESS);
	TWI_Write_data(address);
	TWI_Write_data(data);
	TWI_Stop();
}

uint8_t EEPROM_read_from_address(uint8_t address){
	uint8_t value;
	TWI_Start();
	TWI_Write_SLA_W(EEPROM_ADDRESS);
	TWI_Write_data(address);
	TWI_Stop();
	TWI_Start();
	TWI_Write_SLA_R(EEPROM_ADDRESS);
	value = TWI_Read_ACK();
	TWI_Stop();
	return value;
}

void EEPROM_read_all_data(void){
	uint8_t buff[EEPROM_LAST_REGISTER_ADDR];
	TWI_Start();
	TWI_Write_SLA_W(EEPROM_ADDRESS);
	TWI_Write_data(EEPROM_FIRST_REGISTER_ADDR);
	TWI_Stop();
	TWI_Start();
	TWI_Write_SLA_R(EEPROM_ADDRESS);
	for(int i = EEPROM_FIRST_REGISTER_ADDR; i <= EEPROM_LAST_REGISTER_ADDR; i++){
		buff[i] = TWI_Read_ACK();
	}
	TWI_Stop();
	
	for(int i = EEPROM_FIRST_REGISTER_ADDR; i <= EEPROM_LAST_REGISTER_ADDR; i++){
		USART_PutS("EEPROM_ADDRESS = ");
		USART_PutS("0x");
		USART_PutInt(i,16);
		USART_PutS("    VALUE = 0x");
		USART_PutInt(buff[i],16);
		USART_PutS("\n\r");
	}
	
	
	/*
	for(int i = EEPROM_FIRST_REGISTER_ADDR; i <= EEPROM_LAST_REGISTER_ADDR; i++){
		value = EEPROM_read_from_address(i);
		USART_PutS("EEPROM_ADDRESS = ");
		USART_PutS("0x");
		USART_PutInt(i,16);
		USART_PutS("    VALUE = 0x")
		USART_PutInt(value,16);
		USART_PutS("\n\r");
	}
	*/
}