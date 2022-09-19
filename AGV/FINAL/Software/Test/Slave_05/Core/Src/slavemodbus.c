#include "slavemodbus.h"
#include "main.h"
#include "string.h"
#include "math.h"
#include "crc.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern uint8_t RxBuf[RxBuf_SIZE];
extern uint8_t MainBuf[MainBuf_SIZE];
extern uint8_t slavedata[slavedata_SIZE];
extern bool slave_rx_flag;
extern bool dma_rx_flag;

uint8_t slave_id;

void validate_req(uint8_t RxBuf[],uint8_t len)
{  
		if(RxBuf[0] == slave_id){
		//	RxBuf[0] = 0x00;
			uint16_t u16MsgCRC = ((RxBuf[RxBuf_SIZE - 2] << 8) | RxBuf[RxBuf_SIZE - 1]);  //Combine low and High bytes
			if(CRC_chk(RxBuf,RxBuf_SIZE-2) == u16MsgCRC){
				RxBuf[1] = 0x01;
				if(slave_rx_flag == true){
					slave_rx_flag = false;
				//	if(dma_rx_flag == true){
				//		dma_rx_flag = false;
					delay(15);
				//	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
					HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
					HAL_UART_Transmit(&huart2,slavedata,sizeof(slavedata),1000);
					HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_RESET);
				//	HAL_Delay(100);
				//	}
				}
			}
		}
			else{
				slavedata[0] = slave_id;
				slavedata[1] = 0x09;
				slavedata[2] = 0x00;
				slavedata[3] = 0x00;
				slavedata[4] = 0x00;
				uint16_t crccheck = CRC_chk(slavedata,slavedata_SIZE-2);
				slavedata[5] = crccheck >> 8;
				slavedata[6] = crccheck & 0xFF;
			}
}

void vali_req(uint8_t RxBuf[],uint8_t slave_id)                  //Try../
{
	if(RxBuf[0] == slave_id)
	{
		RxBuf[0] =0x00;  RxBuf[1] = 0x00;
		switch(RxBuf[1])
		{
			case 0x06:
			{
					HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
					HAL_UART_Transmit(&huart2,slavedata,sizeof(slavedata),1000);
					HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_RESET);
			}
			break;
		  default:
			{
				uint8_t errormsg[5] = {slave_id,0x09,0x01};
				uint16_t crccheck = CRC_chk(errormsg,5-2);
				errormsg[3] = crccheck >> 8;
				errormsg[4] = crccheck & 0xFF;
				HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
				HAL_UART_Transmit(&huart2,errormsg,sizeof(errormsg),1000);
				HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_RESET);
			}
			break;
		}
	}
}

void read_sensor(uint8_t RxSensorBuf[],const int HEADER,uint8_t slave_id)
{
			if((RxSensorBuf[0] == HEADER) & (RxSensorBuf[1] == HEADER)){
				uint16_t check = RxSensorBuf[0] + RxSensorBuf[1] + RxSensorBuf[2] + RxSensorBuf[3] + RxSensorBuf[4] + RxSensorBuf[5] + RxSensorBuf[6] + RxSensorBuf[7];
				if(RxSensorBuf[8] == (check & 0xff)){
					uint16_t dist = RxSensorBuf[2] + RxSensorBuf[3] * 256;
					slavedata[0] = slave_id;
					slavedata[1] = 0x06;
					slavedata[2] = 0x07;
					slavedata[3] = dist >> 8;
					slavedata[4] = dist & 0xFF;
					uint16_t crccheck = CRC_chk(slavedata,slavedata_SIZE-2);
					slavedata[5] = crccheck >> 8;
					slavedata[6] = crccheck & 0xFF;
				}
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

