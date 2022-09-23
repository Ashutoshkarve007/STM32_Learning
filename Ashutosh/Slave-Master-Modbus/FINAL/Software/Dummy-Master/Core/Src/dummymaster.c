#include "dummymaster.h"
#include "main.h"
#include "string.h"
#include "crc.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t uart_dma_flag;

void requestdata()
{
	for(uint8_t i = 0x01 ; i < 0x04 ; i++){
  uint8_t datamod[4] = {i,0x06};
	uint16_t crccheck = CRC_chk(datamod,2);
	datamod[2] = crccheck >> 8; 
	datamod[3] = crccheck & 0xFF;
	uint8_t datamodbus[4] = {datamod[0],datamod[1],datamod[2],datamod[3]};
	uart_dma_flag = 1;
	HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
	HAL_UART_Transmit_DMA(&huart2,datamodbus,sizeof(datamodbus));
	while(uart_dma_flag) HAL_Delay(1);
	HAL_Delay(100);  
	}

}



