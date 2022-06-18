
#include "RTC_PCF8563.h"

/* Print data to the COM */
void RTC_print_COM(RTC_data* data_time){
	char buff[100];
	sprintf(buff, "%02d:%02d:%02d	%02d.%02d.%02dr    WEEKDAY:%d\n\r", data_time->hour, data_time->min, 
	data_time->sec, data_time->day, data_time->month, data_time->year, data_time->weekday);
	USART_PutS(buff);
}

/* Print data to the LCD display */
void RTC_print_LCD(RTC_data* data_time){
	char buff[25];
	sprintf(buff, "%02d:%02d:%02d", data_time->hour, data_time->min, data_time->sec);
	LCD1602_goto_xy(0,0);
	LCD1602_send_string(buff);
	sprintf(buff, "%02d.%02d.%02d", data_time->day, data_time->month, data_time->year);
	LCD1602_goto_xy(0,1);
	LCD1602_send_string(buff);

}

/* PCF8563 sends data in BCD code format so we need to convert it to decimal */
uint8_t bcd_to_decimal(uint8_t x) {
	return x - 6 * (x >> 4);
}


/* To send proper value to the RTC while we are setting the time in PCF8563 registers */
uint8_t decimal_to_bcd(uint8_t num){
	uint8_t ones = 0;
	uint8_t tens = 0;
	uint8_t temp = 0;

	ones = num%10; 
	temp = num/10; 
	tens = temp<<4;  
	
	return (tens + ones);
}

/* Get specific value (seconds, minutes etc.) */
uint8_t RTC_get_specific_value(uint8_t register_address){
	uint8_t value;
	TWI_Start(); // I2C START
	TWI_Write_SLA_W(PCF8563_Address); // SLAVE ADDRESS IN WRITE MODE
	TWI_Write_data(register_address); // REGISTER ADDRESS (seconds = VL_SECONDS, minutes = MINUTES)
	TWI_Stop(); // I2C STOP
	TWI_Start(); // I2C START
	TWI_Write_SLA_R(PCF8563_Address); // SLAVE ADDRESS IN READ MODE
	value = TWI_Read_NACK(); // READ DATA AND NACK
	TWI_Stop(); // I2C STOP
	
	if (register_address == VL_SECONDS_REG)
		value &= SECONDS_MASK;			
	else if (register_address == MINUTES_REG)	
		value &= MINUTES_MASK;			
	else if (register_address == HOURS_REG)			
		value &= HOURS_MASK;				
	else if (register_address == DAYS_REG)		
		value &= DAYS_MASK;				
	else if (register_address == WEEKDAYS_REG)	
		value &= WEEKDAYS_MASK;		
	else if (register_address == CENTURY_MONTHS_REG)		
		value &= MONTHS_MASK;			
	else if (register_address == YEARS_REG)			
		value &= YEARS_MASK;
		
	return bcd_to_decimal(value); //PCF8563 sends data in BCD code format so we need to convert it (datasheet)		
}

void RTC_get_time(RTC_data* data){
	uint8_t sec, min, hour, day, weekday, month, year;
	
	/* I2C PROCEDURE */
	TWI_Start(); // START I2C
	TWI_Write_SLA_W(PCF8563_Address); // SLAVE ADDRESS IN WRITE MODE
	TWI_Write_data(VL_SECONDS_REG); // REGISTER ADDRESS
	TWI_Stop(); // STOP I2C
	
	TWI_Start(); // START I2C
	TWI_Write_SLA_R(PCF8563_Address); // SLAVE ADDRESS IN READ MODE
	sec = TWI_Read_ACK(); // READ DATA AND AUTO INCREMENT ADDRESS OF REGISTER
	min = TWI_Read_ACK();
	hour = TWI_Read_ACK();
	day = TWI_Read_ACK();
	weekday = TWI_Read_ACK();
	month = TWI_Read_ACK();
	year = TWI_Read_NACK(); // NACK
	TWI_Stop(); // STOP
	
	/* MASK PROPER BITS */
	sec &= SECONDS_MASK;			// SECONDS[6:0] from datasheet
	min &= MINUTES_MASK;			// MINUTES[6:0] from datasheet
	hour &= HOURS_MASK;				// HOURS[5:0] from datasheet
	day &= DAYS_MASK;				// DAYS[5:0] from datasheet
	weekday &= WEEKDAYS_MASK;		// WEEKDAYS[2:0] from datasheet
	month &= MONTHS_MASK;			// MONTHS[4:0] from datasheet
	year &= YEARS_MASK;				// YEARS[7:0] from datasheet
	
	/* PCF8563 sends data in BCD code format so we need to convert it to decimal */
	sec = bcd_to_decimal(sec);
	min = bcd_to_decimal(min);
	hour = bcd_to_decimal(hour);
	day = bcd_to_decimal(day);
	weekday = bcd_to_decimal(weekday);
	month = bcd_to_decimal(month);
	year = bcd_to_decimal(year);
	
	/* WRITE TO DATA STRUCTURE */
	data->sec = sec;
	data->min = min;
	data->hour = hour;
	data->day = day;
	data->weekday = weekday;
	data->month = month;
	data->year = year;
}

void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint8_t year, uint8_t weekday){
	/* Convert to BCD */
	hour = decimal_to_bcd(hour);
	min = decimal_to_bcd(min);
	sec = decimal_to_bcd(sec);
	day = decimal_to_bcd(day);
	month = decimal_to_bcd(month);
	year = decimal_to_bcd(year);
	weekday = decimal_to_bcd(weekday);
	
	/* Set time by I2C */
	TWI_Start(); // I2C START
	TWI_Write_SLA_W(PCF8563_Address); // SLAVE ADDRESS AND WRITE MODE
	TWI_Write_data(VL_SECONDS_REG); // REGISTER ADDRESS
	TWI_Write_data(sec); // WRITE DATA AND AUTO INCREMENT REGISTER ADDRESS
	TWI_Write_data(min);
	TWI_Write_data(hour);
	TWI_Write_data(day);
	TWI_Write_data(weekday);
	TWI_Write_data(month);
	TWI_Write_data(year);
	TWI_Stop();
}