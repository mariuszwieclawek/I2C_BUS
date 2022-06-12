#include "LCD1602.h"
#include "usart.h"


uint8_t LCD_state = 0x00;


/* Send MSB D7...D4*/
void send_half_byte(char t){
	t <<= 4;
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state |= E); // ENABLE must be set when we write data
	_delay_us(50);
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state | t);
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state &= ~E); // After write data ENABLE bit must be cleared
	_delay_us(50);
}


/* Send command to control display */
void LCD1602_send_command(char command){
	char highc = 0;
	highc = command >> 4; // MSB

	/* When control our display by command RS must be off */
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state &=~ RS);  // RS off

	/* We are initialize our LCD in 4 bits mode so we need first send MSB(D7...D4) then LSB(D3...D0) */
	send_half_byte(highc); // MSB 
	send_half_byte(command); // LSB
}


/* Send character to LCD */
void LCD1602_send_char(char character){
	char highc = 0;
	highc = character >> 4; // MSB
	
	/* When sending or reading character RS must be on */
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state |= RS);   // RS on
	
	/* We are initialize our LCD in 4 bits mode so we need first send MSB(D7...D4) then LSB(D3...D0) */
	send_half_byte(highc);
	send_half_byte(character);
}


/* LCD initialization, it is described in datasheet */
void LCD1602_Init(void){
	TWI_Init();
	_delay_ms(15);
	send_half_byte(LCD_FUNC8B>>4);
	_delay_ms(4.1);
	send_half_byte(LCD_FUNC8B>>4); 
	_delay_us(100);
	send_half_byte(LCD_FUNC8B>>4);
	_delay_ms(1);
	send_half_byte(LCD_FUNC4B>>4);
	_delay_ms(1);
	
	LCD1602_send_command(FUNCTION_SET);
	_delay_ms(1);
	LCD1602_send_command(DISPLAY_ON);
	_delay_ms(1);
	LCD1602_send_command(DISPLAY_CLEAR);
	_delay_ms(1);
	LCD1602_send_command(ENTRY_MODE_SET);
	_delay_ms(1);
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state |= BACKLIGHT_ON);
	TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state &=~ COURSOR_HOME);
}


/* LCD display clear */
void LCD1602_clear(void){
	LCD1602_send_command(DISPLAY_CLEAR);
	_delay_us(1500);
}


/* LCD cursor position control */
void LCD1602_goto_xy(char col, char row){
	char adr;

	adr = 0x40*row + col;
	adr |= 0x80;
	LCD1602_send_command(adr);
}


/* Send text to LCD */
void LCD1602_send_string(const char *str){
	int i;

	for(i = 0; str[i] != '\0'; i++)
		LCD1602_send_char(str[i]);
}


/* Turn on/off backlight */
void LCD1602_backLight(char mode){
	switch(mode) {
		case 1:
			TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state |= BACKLIGHT_ON); //BackLight ON
			break;
		case 0:
			TWI_WriteByAddr(LCD_ADDR_WRITE, LCD_state &= ~BACKLIGHT_ON); //BackLight OFF
			break;
	}
}
