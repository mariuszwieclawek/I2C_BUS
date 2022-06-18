#ifndef MYTWI_H_
#define MYTWI_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>

#include "usart.h"


/* inicjalizacja portow TWI */
void TWI2_Init(void);

/* start modulu twi */
bool TWI2_Start(void);

/* stop modulu twi */
void TWI2_Stop(void);

/* wywolanie slave'a w trybie do zapisu */
bool TWI2_Write_SLA_W(uint8_t slave_address);

/* wywolanie slave'a w trybie do odczytu */
bool TWI2_Write_SLA_R(uint8_t slave_address);

/* wyslanie pojedynczego bajtu */
bool TWI2_Write_data(uint8_t data);

void TWI2_Write_ACK(uint8_t data);

void TWI2_Write_NACK(uint8_t data);

/* odebranie pojedycznego bajtu z potwierdzeniem */
uint8_t TWI2_Read_ACK(void);

/* odebranie pojedycznego bajtu bez potwierdzenia */
uint8_t TWI2_Read_NACK(void);

/* wyslanie pojedynczego bajtu na dany adres */
void TWI2_WriteByAddr(uint8_t addr, uint8_t data);

/* szukanie adresu SLAVE'a*/
void TWI2_Ping_Slave(void);

#endif /* MYTWI_H_ */