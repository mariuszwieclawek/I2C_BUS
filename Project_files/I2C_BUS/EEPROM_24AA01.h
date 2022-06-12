#ifndef EEPROM_24AA01_H_
#define EEPROM_24AA01_H_

#include <stdint.h>
#include "myTWI.h"
#include "usart.h"

#define EEPROM_ADDRESS			0xA0 
#define EEPROM_FIRST_REGISTER_ADDR		0x00
#define EEPROM_LAST_REGISTER_ADDR		0x7F


void EEPROM_write_by_address(uint8_t address, uint8_t data);
uint8_t EEPROM_read_from_address(uint8_t address);
void EEPROM_read_all_data(void);

#endif /* EEPROM_24AA01_H_ */