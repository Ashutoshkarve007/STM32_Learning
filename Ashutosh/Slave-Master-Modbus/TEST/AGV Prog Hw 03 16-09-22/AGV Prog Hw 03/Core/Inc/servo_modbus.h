#include "main.h"

#define servo_id_left		2
#define servo_id_right	1

#define servo_dir_stop		0
#define servo_dir_stop_p	1
#define servo_dir_forw		2
#define servo_dir_back		3
#define servo_dir_left		4
#define servo_dir_right		5

#define max_velocity			10
#define velocity_mult			240

//#define servo_velocity_reg_address		0x1D
#define servo_dir_reg_address					0x30
#define servo_start_stop_reg_address	0x7C
#define servo_start_cmd								0x96
#define servo_stop_cmd								0xD8

#define servo_dir_forw_data_MSB		0x09
#define servo_dir_forw_data_LSB		0x60
#define servo_dir_rev_data_MSB		0xFF
#define servo_dir_rev_data_LSB		0x10
//#define servo_dir_rev_data_LSB		0xA0


#define servo_fun_rd_hld_reg			0x03
#define servo_fun_rd_inp_reg			0x04
#define servo_func_wr_single_reg	0x06
#define servo_fun_wr_mult_reg			0x10

#define modbus_dir_tx 				GPIO_PIN_SET
#define modbus_dir_rx 				GPIO_PIN_RESET

uint16_t CRC_chk (uint8_t *, uint8_t);
	
/* motor id,motor dir,motor task */
void servo_control(uint8_t, uint8_t, uint8_t);
void servo_speed_change(uint8_t, uint8_t, uint8_t);

