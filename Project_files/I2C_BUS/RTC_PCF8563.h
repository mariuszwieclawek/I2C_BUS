/*
 * RTC_PCF8563.h
 *
 * Created: 08.06.2022 14:16:32
 *  Author: Mariusz
 */ 


#ifndef RTC_PCF8563_H_
#define RTC_PCF8563_H_

#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include "myTWI.h"
#include "LCD1602.h"

typedef struct{
	char sec;
	char min;
	char hour;
	char day;
	char weekday;
	char month;
	char year;
}RTC_data;

/* Device/Slave address */
#define PCF8563_Address 0xA2

/* Control registers */
#define CONTROL_STATUS_1 0x00
#define CONTROL_STATUS_2 0x01
#define CLK_CONTROL		 0x0D

/* Time registers */
#define VL_SECONDS_REG		0x02
#define MINUTES_REG			0x03
#define HOURS_REG			0x04
#define DAYS_REG			0x05
#define WEEKDAYS_REG		0x06
#define CENTURY_MONTHS_REG	0x07
#define YEARS_REG			0x08

/* Alarm function registers */
#define MINUTE_ALARM	0x09
#define HOUR_ALARM		0x0A
#define DAY_ALARM		0x0B
#define WEEKDAY_ALARM	0x0C

/* Timer function registers */
#define TIME_CONTROL 0x0E
#define TIMER		 0x0F

/* Masks for read specific data. It is described in datasheet */
#define SECONDS_MASK	0x7F // SECONDS[6:0] from datasheet
#define MINUTES_MASK	0x7F // MINUTES[6:0] from datasheet
#define HOURS_MASK		0x3F // HOURS[5:0] from datasheet
#define DAYS_MASK		0x3F // DAYS[5:0] from datasheet
#define WEEKDAYS_MASK	0x07 // WEEKDAYS[2:0] from datasheet
#define MONTHS_MASK		0x1F // MONTHS[4:0] from datasheet
#define YEARS_MASK		0xFF // YEARS[7:0] from datasheet

/* Print data to the COM */
void RTC_print_COM(RTC_data* data_time);

/* Print data to the LCD display */
void RTC_print_LCD(RTC_data* data_time);

/* For decoding and encoding data */
uint8_t bcd_to_decimal(uint8_t x);
uint8_t decimal_to_bcd(uint8_t x);

/* Get specific value (seconds, minutes etc.) */
uint8_t RTC_get_specific_value(uint8_t register_address);

/* Read values from PCF8563 and save it to the data structure */
void RTC_get_time(RTC_data* data);

/* Write values to PCF8563 */
void RTC_set_time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t weekday, uint8_t month, uint8_t year);

#endif /* RTC_PCF8563_H_ */