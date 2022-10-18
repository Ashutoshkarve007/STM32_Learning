#include "max485_slave.h"

bool 		slave_rx_flag = false;
uint8_t slave_id  		= 0x01;
uint8_t slave_rx[20]	= "Slave 1";
extern uint8_t sensor_packet[20];
uint8_t slave_packet_select = 0;
uint8_t slave_error_tx[20];

void slave_rx_process(){
	slave_rx_flag = false;
	if(slave_rx[0] == slave_id)
	{
		uint16_t crc_received = (slave_rx[2] << 8) | slave_rx[3]; 
		
		if(CRC_chk(slave_rx,2) == crc_received){
			HAL_GPIO_WritePin(MAX3485_DE_GPIO_Port,MAX3485_DE_Pin,GPIO_PIN_SET);
			
			if(slave_rx[1] == slave_cmd)	slave_packet_select = 1;
			
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
  
			HAL_UART_Transmit_DMA(&huart2,sensor_packet,7);
			
			//HAL_GPIO_TogglePin(ID_PIN_1_GPIO_Port,ID_PIN_1_Pin);
		} else {
			HAL_GPIO_WritePin(MAX3485_DE_GPIO_Port,MAX3485_DE_Pin,GPIO_PIN_SET);
			slave_error_tx[0] = slave_id;
			slave_error_tx[1] = 0x09;
			slave_error_tx[2] = 0x00;
			slave_error_tx[3] = 0x00;
			slave_error_tx[4] = 0x00;
			uint16_t crccheck = CRC_chk(slave_error_tx,slavedata_SIZE-2);
			slave_error_tx[5] = crccheck >> 8;
			slave_error_tx[6] = crccheck & 0xFF;
			
			HAL_UART_Transmit_DMA(&huart2,slave_error_tx,6);
		}
	}
}
