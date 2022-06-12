#ifndef MYTWI_H_
#define MYTWI_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

#include "usart.h"

#define SDA PORTC4
#define SCL PORTC5

#define ACK 1
#define NOACK 0

/* inicjalizacja portow TWI */
void TWI_Init(void);

/* start modulu twi */
bool TWI_Start(void);

/* stop modulu twi */
void TWI_Stop(void);

/* wywolanie slave'a w trybie do zapisu */
bool TWI_Write_SLA_W(uint8_t slave_address);

/* wywolanie slave'a w trybie do odczytu */
bool TWI_Write_SLA_R(uint8_t slave_address);

/* wyslanie pojedynczego bajtu */
bool TWI_Write_data(uint8_t data);

void TWI_Write_ACK(uint8_t data);

void TWI_Write_NACK(uint8_t data);

/* odebranie pojedycznego bajtu z potwierdzeniem */
uint8_t TWI_Read_ACK(void);

/* odebranie pojedycznego bajtu bez potwierdzenia */
uint8_t TWI_Read_NACK(void);

/* wyslanie pojedynczego bajtu na dany adres */
void TWI_WriteByAddr(uint8_t addr, uint8_t data);

/* wyslanie wiekszej ilosci bajtow */
void TWI_write_buf(uint8_t SLA, uint8_t data, uint8_t len, uint8_t *buf);

/* szukanie adresu SLAVE'a*/
void TWI_Ping_Slave(void);

#endif /* MYTWI_H_ */