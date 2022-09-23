#include "main.h"


#define ID1 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)
#define ID2 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)
#define ID3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)
#define ID4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)


void validate_req(uint8_t MxBuff[],uint8_t len);
void id_detect(void);
void read_sensor(uint8_t RxSensorBuf[],const int HEADER);
void error_data(uint8_t err);