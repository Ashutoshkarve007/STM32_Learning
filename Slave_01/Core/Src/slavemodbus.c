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

uint8_t slave_id = 0;
uint16_t crccheck;
uint16_t crcrecive;
void slaveid(uint8_t RxBuff[],uint8_t len)
{
	switch(RxBuff[1])
	{

	}
}
void validate_req(uint8_t MxBuff[],uint8_t len)
{  
		if(MainBuf[0] == slave_id){
			uint16_t u16MsgCRC = ((MainBuf[MainBuf_SIZE - 2] << 8) | MainBuf[MainBuf_SIZE - 1]);  //Combine low and High bytes
			if(CRC_chk(MainBuf,MainBuf_SIZE-2) == u16MsgCRC){
				HAL_UART_Transmit_DMA(&huart2,slavedata,sizeof(slavedata));
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

