#include "slavemodbus.h"
#include "main.h"
#include "string.h"
#include "math.h"

extern UART_HandleTypeDef huart2;
extern uint8_t dist[1];
uint8_t dec[1];

void slaveid(uint8_t RxBuff[],uint8_t len)
{
	switch(RxBuff[1])
	{
		case 0x01:
			if(ID1 & ID2 & ID3 & !ID4){
				if(RxBuff[2] == 0x00){
					uint8_t datamodbus[10] = {0x01,0x00,dist[0],0x00};
					HAL_UART_Transmit_DMA(&huart2,datamodbus,sizeof(datamodbus));
			}
		}
		break;
	}
}
void modbus(uint8_t RxBuff[],uint8_t len)
{
	
}

uint8_t bin[4];
uint8_t n;
uint8_t i;

uint8_t a[10]; 

void binaryId()
{
	dec[0] = 0;
	
	if(ID1){
	 bin[0] = 0;
	}else{
	 bin[0] = 1;
	}
	if(ID2){
	 bin[1] = 0;
	}else{
	 bin[1] = 1;
	}
	if(ID3){
	 bin[2] = 0;
	}else{
	 bin[2] = 1;
	}
	if(ID4){
	 bin[3] = 0;
	}else{
	 bin[3] = 1;
	}
	
HAL_Delay(100);	
	n = bin[3] + 10* bin[2] + 100*bin[1] + 1000*bin[0] ;
	
HAL_Delay(100);	
	
	for(i=0;n!=0;i++)
	{
		a[i]=n%10;
		n=n/10;
	}
	n=i;
	for(i=0;i<n;i++)
	{
		dec[0]+=a[i]*pow(2,i);
	}
HAL_Delay(100);
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