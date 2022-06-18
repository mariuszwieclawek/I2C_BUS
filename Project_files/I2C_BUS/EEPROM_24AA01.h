#ifndef EEPROM_24AA01_H_
#define EEPROM_24AA01_H_

#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "myTWI_second.h"
#include "usart.h"
#include "DHT11.h"
#include "RTC_PCF8563.h"

#define EEPROM_ADDRESS			0xA0 
#define EEPROM_FIRST_REGISTER_ADDR		0x00
#define EEPROM_LAST_REGISTER_ADDR		0x7F


void EEPROM_write_by_address(uint8_t address, uint8_t data);
uint8_t EEPROM_read_from_address(uint8_t address);
void EEPROM_read_all_data_to_COM(void);
void EEPROM_write_DHT11(uint8_t address, DHT11_data * data);
DHT11_data EEPROM_read_DHT11(uint8_t address);
void EEPROM_write_RTC(uint8_t address, RTC_data* data);
RTC_data EEPROM_read_RTC(uint8_t address);

#endif /* EEPROM_24AA01_H_ */