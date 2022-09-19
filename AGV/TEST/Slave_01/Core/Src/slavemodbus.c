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
extern uint8_t slaveerrordata[slaveerrordata_SIZE];
extern uint8_t transmit_flag;
extern uint8_t uart_dma_flag;
uint8_t slave_id = 0;

void validate_req(uint8_t MxBuff[],uint8_t len)
{  
		if(MxBuff[0] == slave_id){
			MainBuf[1] = 0x00;
			uint16_t u16MsgCRC = ((MxBuff[len - 2] << 8) | MxBuff[len - 1]);  //Combine low and High bytes
			if(CRC_chk(MxBuff,len-2) == u16MsgCRC){
        uart_dma_flag = 1;
				HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
				if(transmit_flag == 0)
				{
				HAL_UART_Transmit_DMA(&huart2,slavedata,sizeof(slavedata));
				}
				else if(transmit_flag == 1)
				{
					HAL_UART_Transmit_DMA(&huart2,slaveerrordata,sizeof(slaveerrordata));
				}
			
				while(uart_dma_flag){ HAL_Delay(1);}
				HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_RESET);

		}
//			else
//			{
//				
//        error_data(0x03); 
//				uart_dma_flag = 1;
//				HAL_GPIO_WritePin(DE_PIN_GPIO_Port,DE_PIN_Pin,GPIO_PIN_SET);
//				HAL_UART_Transmit_DMA(&huart2,slaveerrordata,sizeof(slaveerrordata));
//				while(uart_dma_flag) HAL_Delay(2);
//			}
		
	}

}

void read_sensor(uint8_t RxSensorBuf[],const int HEADER)
{
	if(RxSensorBuf[0] == HEADER){
			if(RxSensorBuf[1] == HEADER){
				transmit_flag  = 0;
				uint16_t check = RxSensorBuf[0] + RxSensorBuf[1] + RxSensorBuf[2] + RxSensorBuf[3] + RxSensorBuf[4] + RxSensorBuf[5] + RxSensorBuf[6] + RxSensorBuf[7];
				if(RxSensorBuf[8] == (check & 0xff)){
					uint16_t dist = RxSensorBuf[2] + RxSensorBuf[3] * 256;
					slavedata[0]  = slave_id;
					slavedata[1]  = 0x06;
					slavedata[2]  = 0x07;
					slavedata[3]  = dist >> 8;
					slavedata[4]  = dist & 0xFF;
					uint16_t crccheck = CRC_chk(slavedata,slavedata_SIZE-2);
					slavedata[5]  = crccheck >> 8;
					slavedata[6] = crccheck & 0xFF;
				}
			}
			else
			{
				transmit_flag = 1;
        error_data(0x01);               //0x01   Error for sensor
				
			}
		}
}

void error_data(uint8_t err)
{
				slaveerrordata[0] = slave_id;
				slaveerrordata[1] = 0x09;
				slaveerrordata[2] = err;
				uint16_t crccheck = CRC_chk(slaveerrordata,slaveerrordata_SIZE-2);
				slaveerrordata[3] = crccheck >> 8;
				slaveerrordata[4] = crccheck & 0xFF;
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

