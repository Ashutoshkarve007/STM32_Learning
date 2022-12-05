#include "lsm6ds3.h"

uint16_t 	SlaveAddress 			= 0;
uint8_t 	who_am_i = 0;
float 		x,y,z;
uint16_t angle_measured;

 

float x_rad,y_rad;
float x_deg,y_deg;

float myAccangle_x,myAccangle_y,myAccangle_z;
float myAccangle_x_min=0,myAccangle_x_max=0;

uint8_t data[6];

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
	
	temp[0] = 0x4C; //0x8E;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL2_G,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x4A; //0xAC;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL1_XL,1,temp,1,100); 			HAL_Delay(1);
	
	temp[0] = 0x00;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL7_G,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x09;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL8_XL,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x08; //0x03;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_INT1_CTRL,1,temp,1,100);			HAL_Delay(1);
	
	temp[0] = 0x20;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL4_C,1,temp,1,100);				HAL_Delay(1);
	
	temp[0] = 0x80;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_MASTER_CONFIG,1,temp,1,100);	HAL_Delay(1);
	
	
////////////// ## Init Shambavi ##  ////////////

	temp[0] = 0x00;
	HAL_I2C_Mem_Write(&hi2c1,SlaveAddress,LSM6DS3_CTRL6_C,1,temp,1,100);				HAL_Delay(1);

}

uint8_t readAcceleration(){
	
	//HAL_I2C_IsDeviceReady(&hi2c1,SlaveAddress,3,100);																										HAL_Delay(1);
	HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,LSM6DS3_OUTX_L_G,1,data,6,100); 											HAL_Delay(1);
	
	x = ((data[1] << 8) + data[0]) * 4.0f / 32768.0f;
  y = ((data[3] << 8) + data[2]) * 4.0f / 32768.0f;
  z = ((data[5] << 8) + data[4]) * 4.0f / 32768.0f;
	
//	if(x > myAccangle_x_max) 	myAccangle_x_max = x;
//	if(x < myAccangle_x_min) 	myAccangle_x_min = x;
	
	
	return 1;
}

uint8_t accelerationAvailable(){
	uint8_t temp[2];
	HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,LSM6DS3_STATUS_REG,1,temp,1,100);
	
	if((temp[0] & 0x01)) 	return 1;
	else									return 0;
}

uint16_t angle_cal (uint16_t angle_measured){
	uint16_t angle_calculated;
	readAcceleration();
	
//	x = myAcceScaled.x;
//	y = myAcceScaled.y;
//	z = myAcceScaled.z;
	
//	if(x > myAccangle_x_max) 	myAccangle_x_max = x;
//	if(x < myAccangle_x_min) 	myAccangle_x_min = x;
	
	int xAng = map(x,minVal,maxVal,-90,90);
	int yAng = map(y,minVal,maxVal,-90,90);
	int zAng = map(z,minVal,maxVal,-90,90);
	
	x_rad = atan2(-yAng, -zAng)+PI;
	y_rad = atan2(-xAng, -zAng)+PI;
	
	x_deg = rad2deg * x_rad;//(atan2(-yAng, -zAng)+PI);
	y_deg = rad2deg * y_rad;//(atan2(-xAng, -zAng)+PI);
	
	if(x_deg > 359.9f) x_deg = 0.0f;
	if(y_deg > 359.9f) y_deg = 0.0f;
	
	//angle_calculated = cos(x_rad)*angle_measured;
	//angle_calculated = cos(y_rad)*angle_measured;
	
	return angle_calculated;
}

//------try------//
//void calc_xy_angles(void){
//   // Using x y and z from accelerometer, calculate x and y angles
//   //float x_val, y_val, z_val, result;
//  
//    unsigned long xsq, ysq, zsq; //24 bit
//   // Lets get the deviations from our baseline
//   x = (float)accel_value_x-(float)accel_center_x;
//   y = (float)accel_value_y-(float)accel_center_y;
//   z = (float)accel_value_z-(float)accel_center_z;

//   // Work out the squares
//   x2 = (unsigned short long)(x_val*x_val);
//   y2 = (unsigned short long)(y_val*y_val);
//   z2 = (unsigned short long)(z_val*z_val);

//   //X Axis
//   result=sqrt(y2+z2);
//   result=x_val/result;
//   accel_angle_x = atan(result);

//   //Y Axis
//   result=sqrt(x2+z2);
//   result=y_val/result;
//   accel_angle_y = atan(result);
//}
