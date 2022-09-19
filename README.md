# STM32_Learning
 Learn STM32 together. 
 

*Slave Board*

pin 1 - VCC     Modbus side
pin 2 - DO
pin 3 - CLK
pin 4 - rst*
pin 5 - GND


Master - Slave

Request Data - {slave_id, 0x06, CRC_L, CRC_H}
