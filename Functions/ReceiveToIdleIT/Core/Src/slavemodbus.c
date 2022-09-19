#include "slavemodbus.h"
#include "main.h"
#include "string.h"
#include "math.h"

extern UART_HandleTypeDef huart2;
extern uint8_t dist[1];
uint8_t dec[1];
uint8_t slave_id = 0;

void slaveid(uint8_t RxBuff[],uint8_t len)
{
	switch(RxBuff[1])
	{
		case 0x01:
			if(ID1 & ID2 & ID3 & !ID4){
				if(RxBuff[2] == 0x00){
					uint8_t datamodbus[10] = {slave_id,0x00,dist[0],0x00};
					HAL_UART_Transmit_DMA(&huart2,datamodbus,sizeof(datamodbus));
			}
		}
		break;
	}
}
void modbus(uint8_t RxBuff[],uint8_t len)
{
	
}


void checkboard()
{
	if(ID1){
		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,1);
	}
	else
	{ 
		HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,0);
	}
}

void id_detect()
{
	if(ID1 == SET){
		slave_id = slave_id | 0x01;
	}
	if(ID2 == SET){
		slave_id = slave_id | 0x02;
	}
	if(ID3 == SET){
		slave_id = slave_id | 0x04;
	}
	if(ID4 == SET){
		slave_id = slave_id | 0x08;
	}
}