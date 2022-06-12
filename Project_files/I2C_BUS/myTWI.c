
#define TWSR TWSR0
#include <util/twi.h>
#include "myTWI.h"

#define F_CPU 16000000UL /* define CPU frequency in hz here if not defined in Makefile */
#define SCL_CLOCK  100000L /* I2C clock in Hz */

void TWI_Init(void) {
	PRR0 &= ~(1<<PRTWI0);
	//DDRC |= (1<<SDA) | (1<<SCL); //enable ports
	
	/* SETTING SCL CLOCK:
	* SCL = F_CPU/(16+2(TWBR0)*PrescalerValue)
	* F_CPU 16MHz 
	* TWBR0 = 0x48 ( dec = 72 )
	* default precaler value = 1
	* SCL CLK = 100khz 
	*/
	//TWCR0 = (1<<TWEA) | (1<<TWEN); //W³¹cz interfejs I2C
	/* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
	//TWSR0 = 0;                         /* no prescaler */
	TWBR0 = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */
	
	//TWBR0 = 0x48;
}

bool TWI_Start(void) {
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	while(!(TWCR0 & (1<<TWINT)));
	if (TW_STATUS == TW_START){
		return true; // ACK
	}		else{		USART_PutS("START NACK\n\r");		return false; // NACK	}									
}

void TWI_Stop(void) {
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while(TWCR0 & (1<<TWSTO)); // wait until stop condition is executed and bus released
}

bool TWI_Write_SLA_W(uint8_t slave_address){ /* MASTER TRANSMITTER */ 
	TWDR0 = slave_address;
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR0 & (1<<TWINT)));
	if (TW_STATUS == TW_MT_SLA_ACK){
		return true; // ACK	}	else{				USART_PutS("Write_SLA_W NACK\n\r");								return false; // NACK
	}
}


bool TWI_Write_SLA_R(uint8_t slave_address){ /* MASTER RECEIVER */
	slave_address |= 0x01;
	TWDR0 = slave_address;
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR0 & (1<<TWINT)));
	if ((TWSR0 & 0xF8) == TW_MR_SLA_ACK){
		return true; // ACK	}	else{		USART_PutS("Write_SLA_R NACK\n\r");		return false; // NACK
	}
}

bool TWI_Write_data(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR0 & (1<<TWINT)));
	if ((TWSR0 & 0xF8) == TW_MT_DATA_ACK){
		return true; // ACK	}	else{		USART_PutS("Write_data NACK\n\r");						  		return false; // NACK
	}
}

void TWI_Write_ACK(uint8_t data) {
	TWDR0 = data;
	TWCR0 = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
	while(!(TWCR0 & (1<<TWINT)));
}

void TWI_Write_NACK(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR0 & (1<<TWINT)));
}

uint8_t TWI_Read_ACK(void)
{
	//USART_PutS("read11\n\r");
	TWCR0 = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
	//USART_PutS("read12\n\r");
	while (!(TWCR0 & (1<<TWINT)));
	//USART_PutS("read13\n\r");
	return TWDR0;
}

uint8_t TWI_Read_NACK(void)
{
	TWCR0 = ((1 << TWINT) | (1 << TWEN)) ;
	while (!(TWCR0 & (1<<TWINT)));
	return TWDR0;
}

void TWI_Ping_Slave(void){
	for (int slave_address = 0x00 ; slave_address <= 0xFF; slave_address++){
		TWI_Start();
		if (TWI_Write_SLA_W(slave_address) == true)
		{
			USART_PutS("\r\n\tSlave address: ");
			USART_PutInt(slave_address,16);
			USART_PutS("\n\r\n\r");
			TWI_Stop();
			continue;
		}
		else{
			USART_PutS("Cant find: ");
			USART_PutInt(slave_address,16);
			USART_PutS("\n\r");
		}
		TWI_Stop();
	}
}


void TWI_WriteByAddr(uint8_t addr, uint8_t data) {
	TWI_Start();
	TWI_Write_SLA_W(addr);
	TWI_Write_data(data);
	TWI_Stop();
}

/*
void TWI_write_buf(uint8_t SLA, uint8_t data, uint8_t len, uint8_t *buf) {
	TWI_Start();
	TWI_Write(SLA);
	TWI_Write(data);
	while(len--) TWI_Write(*buf++);
	TWI_Stop();
}
*/
