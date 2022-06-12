#include "AM2320.h"

void AM2320_print_COM(void){
	char buff[100];
	sprintf(buff, "Humidity = %03d.%02d%%	Temperature = %02d.%02d\n\r", sensor.high_humidity, sensor.low_humidity, sensor.high_temperature, sensor.low_temperature);
	USART_PutS(buff);
}

void AM2320_print_LCD(void){
	char buff[100];
	sprintf(buff, "H=%02d.%02d  T=%02d.%02d", sensor.high_humidity, sensor.low_humidity, sensor.high_temperature, sensor.low_temperature);
	LCD1602_goto_xy(0,1);
	LCD1602_send_string(buff);
}

void AM2320_Init()
{
	unsigned char s = 0;
	
	for(s = 0; s< 8; s++)
	{
		data_buffer[s] = 0x00;
	}
	
	TWI_Init();
}

/*
void AM2320_write(unsigned char *value, unsigned char length)
{
	unsigned char s = 0x00;
	
	TWI_Start();
	TWI_Write(AM2320_address + I2C_write_cmd);
	
	for(s = 0x00; s < length; s++)
	{
		TWI_Write(*value++);
	}
	
	TWI_Stop();
}


void AM2320_read(unsigned char *value, unsigned char length)
{
	unsigned char s = 0x00;

	TWI_Start();
	TWI_Write(AM2320_address + I2C_read_cmd);
	
	for(s = 0x00; s < length; s++)
	{
		value[s] = TWI_Read_ACK();
	}
	
	TWI_Stop();
}
*/

unsigned int CRC16(unsigned char *ptr, unsigned char length)
{
	unsigned int crc = 0xFFFF;
	unsigned char s = 0x00;

	while(length--)
	{
		crc ^= *ptr++;
		for(s = 0; s < 8; s++)
		{
			if((crc & 0x01) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}

	return crc;
}

void AM2320_wake_sensor(void){
	/* Wake Sensor */
	TWI_Start(); // START
	TWI_Write_SLA_W(0xB8); // (I2C address + W(0) ) -write
	_delay_us(800); // wait > 800us & wait < 3ms
	TWI_Stop(); // STOP
}

void AM2320_frame_format(void){
	/* The host sends reading frame format: */
	if(TWI_Start()); // START
	TWI_Write_SLA_W(0xB8); // (I2C address + W(0) ) -write
	TWI_Write_data(0x03); // function code (0x03)
	TWI_Write_data(0x00); // start address
	TWI_Write_data(0x04); // Read the number of register // HH/LH, HT,LT
	TWI_Stop(); // STOP
	_delay_us(1600);
}

void AM2320_read_data(void){
	TWI_Start(); // START
	TWI_Write_SLA_R(0xB8); // (I2C address + W(1) ) - read
	//_delay_us(30);
	sensor.func_code = TWI_Read_ACK();
	sensor.return_numb_of_bytes = TWI_Read_ACK();
	sensor.high_humidity = TWI_Read_ACK();
	sensor.low_humidity = TWI_Read_ACK();
	sensor.high_temperature = TWI_Read_ACK();
	sensor.low_temperature = TWI_Read_ACK();
	sensor.CRC_code = TWI_Read_ACK();
	TWI_Stop(); // STOP
}

void AM2320_my_read(void){
	AM2320_wake_sensor();
	AM2320_frame_format();
	AM2320_read_data();
}


void my_read_data(void)
{
	/* Wake Sensor */
	USART_PutS("test11\n\r");
	TWI_Start(); // START
	USART_PutS("test22\n\r");
	TWI_Write_ACK(0xB8); // (I2C address + W(0) ) -write
	_delay_us(800); // wait > 800us & wait < 3ms
	USART_PutS("test33\n\r");
	TWI_Stop(); // STOP
	
	
	/* The host sends reading frame format: */
	USART_PutS("test1\n\r");
	TWI_Start(); // START
	USART_PutS("test2\n\r");
	TWI_Write_ACK(0xB8); // (I2C address + W(0) ) -write
	//_delay_ms(1.5);
	USART_PutS("test3\n\r");
	TWI_Write_ACK(0x03); // function code (0x03)
	USART_PutS("test4\n\r");
	TWI_Write_ACK(0x00); // start address
	USART_PutS("test6\n\r");
	TWI_Write_ACK(0x04); // Read the number of register // HH/LH, HT,LT
	USART_PutS("test7\n\r");
	TWI_Stop(); // STOP
	_delay_us(1600);
	
	/* Host read return data: */
	USART_PutS("test8\n\r");
	TWI_Start(); // START
	USART_PutS("test9\n\r");
	TWI_Write_ACK(0xB8 | 0x01); // (I2C address + W(1) ) - read
	//_delay_us(30);
	USART_PutS("test10\n\r");
	sensor.func_code = TWI_Read_ACK();
	USART_PutS("test11\n\r");
	sensor.return_numb_of_bytes = TWI_Read_ACK();
	USART_PutS("test12\n\r");
	sensor.high_humidity = TWI_Read_ACK();
	USART_PutS("test13\n\r");
	sensor.low_humidity = TWI_Read_ACK();
	USART_PutS("test14\n\r");
	sensor.high_temperature = TWI_Read_ACK();
	USART_PutS("test15\n\r");
	sensor.low_temperature = TWI_Read_ACK();
	USART_PutS("test16\n\r");
	sensor.CRC_code = TWI_Read_ACK();
	USART_PutS("test17\n\r");
	TWI_Stop(); // STOP
}

void get_sensor_data(unsigned char start_address, unsigned char input_length, unsigned char output_length)
{
	unsigned char s = 0x00;

	AM2320_write(0x00, 0x00);
	_delay_us(1600);

	data_buffer[0] = AM2320_read_sensor_data;
	data_buffer[1] = start_address;
	data_buffer[2] = input_length;

	AM2320_write(data_buffer, 0x03);
	_delay_us(1499);

	for(s = 0x00; s < output_length; s++)
	{
		data_buffer[s] = 0x00;
	}

	AM2320_read(data_buffer, output_length);
}


void get_RH_and_temperature(unsigned int *data1, signed int *data2)
{
	*data1 = ((unsigned int)((data_buffer[2] << 8) | data_buffer[3]));
	*data2 = ((data_buffer[4] << 8) | data_buffer[5]);
}


void get_CRC(unsigned int *CRC_data)
{
	*CRC_data = ((unsigned int)((data_buffer[7] << 8) | data_buffer[6]));
}