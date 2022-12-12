#include "lsm6ds3.h"

uint16_t 	SlaveAddress 			= 0;
uint8_t 	who_am_i = 0;
float 		x,y,z;

int16_t data[3];
	
void find_LSM_address(){
	for(SlaveAddress = 0; SlaveAddress < 0xFF; SlaveAddress++)
	{
		if (HAL_I2C_IsDeviceReady(&hi2c1,SlaveAddress,3,100) == HAL_OK){
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
			HAL_Delay(10);	
			break;
		}
	}	
}

void LSM_init(){
	uint8_t temp[2];
	HAL_I2C_IsDeviceReady(&hi2c1,SlaveAddress,3,100);																										HAL_Delay(1);
	//HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,0xAA,1,Touch_Status,10,10);
	//HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,0xAA,1,(uint8_t *) LSM6DS3_WHO_AM_I_REG,10,10);
	
	HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,LSM6DS3_WHO_AM_I_REG,1,temp,1,100); 														HAL_Delay(1);
	
	who_am_i = temp[0];
	
	temp[0] = 0x8E;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL2_G,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0xAC;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL1_XL,1,temp,1,100); 			HAL_Delay(1);
	
	temp[0] = 0x00;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL7_G,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x09;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL8_XL,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x03;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_INT1_CTRL,1,temp,1,100);			HAL_Delay(1);
	
	temp[0] = 0x20;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL4_C,1,temp,1,100);				HAL_Delay(1);
	
//	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL2_G,1,(uint8_t *) LSM6DS3_data_gyro,1,100);				HAL_Delay(1);
//	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL1_XL,1,(uint8_t *) LSM6DS3_data_acce,1,100); 			HAL_Delay(1);
//	
//	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL7_G,1,(uint8_t *) LSM6DS3_data_gyro_mode,1,100);	HAL_Delay(1);
//	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL8_XL,1,(uint8_t *) LSM6DS3_data_odr_conf,1,100);	HAL_Delay(1);
}

uint8_t readAcceleration(){
	
	
	HAL_I2C_IsDeviceReady(&hi2c1,SlaveAddress,3,100);																										HAL_Delay(1);
	HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,LSM6DS3_OUTX_L_G,1,(uint8_t*)data,6,100); 											HAL_Delay(1);
	
	x = data[0] * 4.0 / 32768.0;
  y = data[1] * 4.0 / 32768.0;
  z = data[2] * 4.0 / 32768.0;
	return 1;
}

uint8_t accelerationAvailable(){
	uint8_t temp[2];
	HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,LSM6DS3_STATUS_REG,1,temp,1,100);
	
	if((temp[0] & 0x01)) 	return 1;
	else									return 0;
}

