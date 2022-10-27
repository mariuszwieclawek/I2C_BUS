# I2C_BUS
Based on Microchip ATmega328PB Xplained Mini Evaluation Kit. Project shows the operation of the I2C bus in the MASTER-SLAVE architecture.
SLAVE devices will be used, such as: temperature and humidity sensor (AM2320), LCD1602 display with I2C expander, Real Time Clock PCF8563 
and external EEPROM memory. The data will be buffered in a ring buffer and sent via USART and I2C interface to the display for data visualization. 
The data is saving to the EEPROM memory and cyclic reading to the putty terminal.

# Documentation 
[documentation.pdf](https://github.com/mariuszwieclawek/I2C_BUS/files/9794968/documentation.pdf)
