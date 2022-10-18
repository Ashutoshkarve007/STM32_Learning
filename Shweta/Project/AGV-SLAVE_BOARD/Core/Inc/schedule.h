#include "main.h"
#include "tim.h"
#include "usart.h"

#define ID1 						HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)
#define ID2 						HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)
#define ID3 						HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define ID4 						HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)

#define sensor_stat_ok 	1


#define slavedata_SIZE 	7
#define	ms_count				10
#define	HEADER 					0x59

void 		slave_start(void);
uint8_t	id_detect(void);
void 		process_sensor(void);
void 		schedule(void);
