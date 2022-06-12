#ifndef AM2320_H_
#define AM2320_H_

#define F_CPU 16000000UL

#include <stdio.h>
#include "myTWI.h"
#include "util/delay.h"
#include "usart.h"
#include "LCD1602.h"

#define AM2320_address                            0xB8
#define I2C_write_cmd                             0x00
#define I2C_read_cmd                              0x01

#define AM2320_read_sensor_data                   0x03
#define AM2320_write_multiple_registers           0x10

#define AM2320_RH_hb                              0x00
#define AM2320_RH_lb                              0x01
#define AM2320_T_hb                               0x02
#define AM2320_T_lb                               0x03

unsigned char data_buffer[8];

struct data{
	uint8_t func_code;
	uint8_t return_numb_of_bytes;
	uint8_t high_humidity;
	uint8_t low_humidity;
	uint8_t high_temperature;
	uint8_t low_temperature;
	uint8_t CRC_code;
}sensor;

void AM2320_init();
void AM2320_write(unsigned char *value, unsigned char length);
void AM2320_read(unsigned char *value, unsigned char length);
unsigned int CRC16(unsigned char *ptr, unsigned char length);
void get_sensor_data(unsigned char start_address, unsigned char input_length, unsigned char output_length);
void get_RH_and_temperature(unsigned int *data1, signed int *data2);
void get_CRC(unsigned int *CRC_data);


void my_read_data(void);
void AM2320_my_read(void);
void AM2320_print_COM(void);
void AM2320_print_LCD(void);

#endif /* AM2320_H_ */