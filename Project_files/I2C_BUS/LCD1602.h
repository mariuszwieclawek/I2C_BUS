#ifndef LCD1602_H_
#define LCD1602_H_

#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdbool.h>

#include "myTWI.h"
#include "usart.h"


/* Slave address */
#define PCF8574T_ADDR 0x27
// We need shift PCF8574T address to add write bit (for I2C communication)
#define LCD_ADDR_WRITE PCF8574T_ADDR<<1


/* Bits defines in 4bits control mode */
#define RS 0x01
#define RW 0x02
#define E  0x04
#define K  0x08
#define D4 0x10
#define D5 0x20
#define D6 0x40
#define D7 0x80


/* For initialization */
#define LCD_FUNC	0x00
#define LCD_FUNC8B	0x30
#define LCD_FUNC4B	0x20

#define FUNCTION_SET	0x28 // 2 rows display, 5x7 dots,
#define DISPLAY_ON		0x0C // display on, cursor off
#define DISPLAY_CLEAR	0x01 // display clear
#define ENTRY_MODE_SET	0x06 // the cursor or display window moves to the right, cursor shift/ window stay

/* Command control */
#define BACKLIGHT_ON	0x08
#define COURSOR_HOME	0x02



/* Send MSB D7...D4*/
void send_half_byte(char t);


/* Send command to control display */
void LCD1602_send_command(char command);


/* Send character to LCD */
void LCD1602_send_char(char character);


/* LCD initialization, it is described in datasheet */
void LCD1602_Init(void);

/* LCD display clear */
void LCD1602_clear(void);


/* LCD cursor position control */
void LCD1602_goto_xy(char row, char col);


/* Send text to LCD */
void LCD1602_send_string(const char *str);


/* Turn on/off backlight */
void LCD1602_backLight(char mode);

#endif /* LCD1602_H_ */