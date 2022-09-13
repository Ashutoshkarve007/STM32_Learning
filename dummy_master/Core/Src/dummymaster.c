#include "dummymaster.h"
#include "main.h"
#include "string.h"
#include "crc.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
uint16_t crccheck;

void requestdata()
{
	uint8_t datamod[4] = {0x01,0x06};
	crccheck = CRC_chk(datamod,2);
	datamod[2] = crccheck >> 8; 
	datamod[3] = crccheck & 0xFF;
	uint8_t datamodbus[4] = {datamod[0],datamod[1],datamod[2],datamod[3]};
	HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,1);
	HAL_UART_Transmit_DMA(&huart2,datamodbus,sizeof(datamodbus));
	HAL_Delay(100);
}



