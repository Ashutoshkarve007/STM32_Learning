#include "main.h"


#define ID1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)
#define ID2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)
#define ID3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define ID4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)

void slaveid(uint8_t RxBuff[],uint8_t len);
	
void modbus(uint8_t RxBuff[],uint8_t len);

void checkboard();
void binaryId();

void id_detect();