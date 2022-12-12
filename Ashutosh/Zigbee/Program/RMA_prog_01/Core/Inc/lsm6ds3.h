#include "main.h"
#include "i2c.h"

#define LSM6DS3_ADDRESS            0x6B

#define LSM6DS3_INT1_CTRL					 0x0D	
#define LSM6DS3_INT2_CTRL					 0x0E	

#define LSM6DS3_WHO_AM_I_REG       0X0F
#define LSM6DS3_CTRL1_XL           0X10
#define LSM6DS3_CTRL2_G            0X11

#define LSM6DS3_STATUS_REG         0X1E

#define LSM6DS3_CTRL4_C            0X13
#define LSM6DS3_CTRL6_C            0X15
#define LSM6DS3_CTRL7_G            0X16
#define LSM6DS3_CTRL8_XL           0X17

#define LSM6DS3_OUTX_L_G           0X22
#define LSM6DS3_OUTX_H_G           0X23
#define LSM6DS3_OUTY_L_G           0X24
#define LSM6DS3_OUTY_H_G           0X25
#define LSM6DS3_OUTZ_L_G           0X26
#define LSM6DS3_OUTZ_H_G           0X27

#define LSM6DS3_OUTX_L_XL          0X28
#define LSM6DS3_OUTX_H_XL          0X29
#define LSM6DS3_OUTY_L_XL          0X2A
#define LSM6DS3_OUTY_H_XL          0X2B
#define LSM6DS3_OUTZ_L_XL          0X2C
#define LSM6DS3_OUTZ_H_XL          0X2D

#define LSM6DS3_data_gyro					 0x4C
#define LSM6DS3_data_acce					 0x4A
#define LSM6DS3_data_gyro_mode		 0x00
#define LSM6DS3_data_odr_conf 		 0x09

void find_LSM_address(void);
void LSM_init(void);
uint8_t readAcceleration(void);
uint8_t accelerationAvailable(void);


