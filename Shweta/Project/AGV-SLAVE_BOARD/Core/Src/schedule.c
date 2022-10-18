#include "schedule.h"

//const int HEADER = 0x59;

extern uint8_t slave_id;
extern uint8_t slave_rx[20];
extern uint8_t slave_tx[20];

uint8_t	sensor_rx[20];
uint8_t sensor_packet[20];

bool 		ms_flag 		= false;
uint8_t	sensor_stat	=	0;
uint8_t	ms			=	0;

bool 			sensor_rx_flag = false;
uint16_t 	dist = 0;

void slave_start(){
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,slave_rx,20);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,sensor_rx,20);
}

uint8_t id_detect()
{
	uint8_t slave = 0;
	if(ID1 == SET)	slave = slave | 0x01;
	if(ID2 == SET)	slave = slave | 0x02;
	if(ID3 == SET)	slave = slave | 0x04;
	if(ID4 == SET)	slave = slave | 0x08;
	return slave;
}

void process_sensor()
{
	sensor_rx_flag = false;
	if((sensor_rx[0] == HEADER) & (sensor_rx[1] == HEADER)){
		
		uint16_t check = sensor_rx[0] + sensor_rx[1] + sensor_rx[2] + sensor_rx[3] + sensor_rx[4] + sensor_rx[5] + sensor_rx[6] + sensor_rx[7];
		if(sensor_rx[8] == (check & 0xff)){
			sensor_stat = sensor_stat_ok;
			dist = sensor_rx[2] + sensor_rx[3] * 256;
			sensor_packet[0] = slave_id;
			sensor_packet[1] = 0x06;
			sensor_packet[2] = 0x07;
			sensor_packet[3] = dist >> 8;
			sensor_packet[4] = dist & 0xFF;
			uint16_t crccheck = CRC_chk(sensor_packet,slavedata_SIZE-2);
			sensor_packet[5] = crccheck >> 8;
			sensor_packet[6] = crccheck & 0xFF;
		}
	}
}

void schedule(){
	ms_flag = false;
	
	ms++;
	if(ms >= ms_count){
		ms = 0;
	
		
	}
}
