
#define TWSR TWSR1
#include <util/twi.h>
#include "myTWI_second.h"

#define F_CPU 16000000UL /* define CPU frequency in hz here if not defined in Makefile */
#define SCL_CLOCK  100000L /* I2C clock in Hz */

void TWI2_Init(void) {
	PRR1 &= ~(1<<PRTWI1);
	//DDRC |= (1<<SDA) | (1<<SCL); //enable ports
	
	/* SETTING SCL CLOCK:
	* SCL = F_CPU/(16+2(TWBR0)*PrescalerValue)
	* F_CPU 16MHz 
	* TWBR0 = 0x48 ( dec = 72 )
	* default precaler value = 1
	* SCL CLK = 100khz 
	*/
	TWCR1 = (1<<TWEA1) | (1<<TWEN1); //W³¹cz interfejs I2C
	/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
	TWSR1 = 0;                         /* no prescaler */
	TWBR1 = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */
	
	//TWBR0 = 0x48;
}

bool TWI2_Start(void) {
	TWCR1 = (1<<TWINT1) | (1<<TWEN1) | (1<<TWSTA1);
	while(!(TWCR1 & (1<<TWINT1)));
	if ((TWSR1 & 0xF8) == TW_START){
		return true; // ACK
	}		else{		USART_PutS("START NACK\n\r");		return false; // NACK	}									
}

void TWI2_Stop(void) {
	TWCR1 = (1<<TWINT1) | (1<<TWEN1) | (1<<TWSTO1);
	while(TWCR1 & (1<<TWSTO1)); // wait until stop condition is executed and bus released
}

bool TWI2_Write_SLA_W(uint8_t slave_address){ /* MASTER TRANSMITTER */ 
	TWDR1 = slave_address;
	TWCR1 = (1<<TWINT1) | (1<<TWEN1);
	while(!(TWCR1 & (1<<TWINT1)));
	if ((TWSR1 & 0xF8) == TW_MT_SLA_ACK){
		return true; // ACK	}	else{				USART_PutS("Write_SLA_W NACK\n\r");								return false; // NACK
	}
}


bool TWI2_Write_SLA_R(uint8_t slave_address){ /* MASTER RECEIVER */
	slave_address |= 0x01;
	TWDR1 = slave_address;
	TWCR1 = (1<<TWINT1) | (1<<TWEN1);
	while(!(TWCR1 & (1<<TWINT1)));
	if ((TWSR1 & 0xF8) == TW_MR_SLA_ACK){
		return true; // ACK	}	else{		USART_PutS("Write_SLA_R NACK\n\r");		return false; // NACK
	}
}

bool TWI2_Write_data(uint8_t data) {
	TWDR1 = data;
	TWCR1 = (1<<TWINT1) | (1<<TWEN1);
	while(!(TWCR1 & (1<<TWINT1)));
	if ((TWSR1 & 0xF8) == TW_MT_DATA_ACK){
		return true; // ACK	}	else{		USART_PutS("Write_data NACK\n\r");						  		return false; // NACK
	}
}

void TWI2_Write_ACK(uint8_t data) {
	TWDR1 = data;
	TWCR1 = ((1 << TWINT1) | (1 << TWEN1) | (1 << TWEA1));
	while(!(TWCR1 & (1<<TWINT1)));
}

void TWI2_Write_NACK(uint8_t data) {
	TWDR1 = data;
	TWCR1 = (1<<TWINT1) | (1<<TWEN1);
	while(!(TWCR1 & (1<<TWINT1)));
}

uint8_t TWI2_Read_ACK(void){
	TWCR1 = ((1 << TWINT1) | (1 << TWEN1) | (1 << TWEA1));
	while (!(TWCR1 & (1<<TWINT1)));
	return TWDR1;
}

uint8_t TWI2_Read_NACK(void){
	TWCR1 = ((1 << TWINT1) | (1 << TWEN1)) ;
	while (!(TWCR1 & (1<<TWINT1)));
	return TWDR1;
}

void TWI2_Ping_Slave(void){
	char buff[50];
	for (int slave_address = 0x00 ; slave_address <= 0xFF; slave_address++){
		TWI2_Start();
		if (TWI2_Write_SLA_W(slave_address) == true){
			sprintf(buff, "\r\n\tSlave address: 0x%02x\n\r\n\r", slave_address);
			USART_PutS_without_ringbuf(buff);
			TWI2_Stop();
			continue;
		}
		else{
			sprintf(buff, "Cant find: 0x%02x\n\r", slave_address);
			USART_PutS_without_ringbuf(buff);
		}
		TWI2_Stop();
	}
}


void TWI2_WriteByAddr(uint8_t addr, uint8_t data) {
	TWI2_Start();
	TWI2_Write_SLA_W(addr);
	TWI2_Write_data(data);
	TWI2_Stop();
}
