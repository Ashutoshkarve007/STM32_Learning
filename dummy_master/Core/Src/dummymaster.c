#include "dummymaster.h"
#include "main.h"
#include "string.h"
#include "crc.h"

extern UART_HandleTypeDef huart1;


void requestdata()
{
	uint8_t datamodbus[2] = {0x01,0x06};
	HAL_UART_Transmit_DMA(&huart1,datamodbus,sizeof(datamodbus));
}



