

uint16_t crccheck;

void requestdata()
{
	
	uint8_t datamod[4] = {0x01,0x06};
	crccheck = CRC_chk(datamod,2);
	datamod[2] = crccheck >> 8;                                                           // Splitting of bytes by shifting
	datamod[3] = crccheck & 0xFF;
	uint8_t datamodbus[4] = {datamod[0],datamod[1],datamod[2],datamod[3]};
	HAL_UART_Transmit_DMA(&huart1,datamodbus,sizeof(datamodbus));
}


Read this blog...
to understand better...
https://stackoverflow.com/questions/53367838/how-to-split-16-value-into-two-8-bit-values-in-c

