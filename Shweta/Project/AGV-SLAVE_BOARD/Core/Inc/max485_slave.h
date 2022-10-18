#include "main.h"
#include "usart.h"

#define slave_cmd	0x06

#define slave_error_crc_missmatch				0x20
#define slave_error_sensor_noresponse		0x30


void slave_rx_process(void);
