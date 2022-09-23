#include "main.h"
#include "obstacle_sensor.h"
#include "schedule.h"

bool obstacle_sensor_flag = false;

uint32_t car_sensor[35];
uint32_t car_data[35];
uint16_t car_sensor_value;

uint8_t sensor_front,sensor_back;
uint8_t obstacles_front,obstacles_back;

extern uint8_t agv_speed;
extern uint8_t motor_status,agv_moving_speed;

void obstacle_sensor_init(){
	HAL_TIM_IC_Start_DMA(&htim16,TIM_CHANNEL_1,car_sensor,34);
}

void obstacle_check(){
	busy_check(busy_set);
	obstacle_sensor_flag = false;
	
	car_sensor_value = 0;															// Clear Previous
	
	//HAL_GPIO_TogglePin(LED4_GPIO_Port,LED4_Pin);
	
	car_data[0] = car_sensor[0]- car_sensor[33];
	for(uint8_t i = 1; i < 34; i++){
		if(car_sensor[i-1] <  car_sensor[i])		car_data[i] = car_sensor[i] - car_sensor[i-1];
		else																		car_data[i] = (0xFFFF - car_sensor[i-1]) + car_sensor[i];
	}
	
	if(car_data[1] < 1790 || car_data[1] > 1800){
		HAL_TIM_IC_Stop_DMA(&htim16,TIM_CHANNEL_1);
//		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		HAL_Delay(40);
		HAL_TIM_IC_Start_DMA(&htim16,TIM_CHANNEL_1,car_sensor,34);
		return;
	}
	
	for(uint8_t i = 1 ; i < 34; i = i + 2) {
		car_sensor_value = car_sensor_value << 1;
		if(car_data[i] < 400) car_sensor_value++;
	}
	
	sensor_front 	= car_sensor_value >> 8;
	sensor_back		= car_sensor_value & 0xFF;
	
	if( sensor_front > 0 && sensor_front < 0xE5) sensor_front = 0xFF;
	if( sensor_back  > 0 && sensor_back  < 0xE5) sensor_back  = 0xFF;
	
	
	
//	if(sensor_front >= mtr_stop) 	 {obstacles_front = 5; agv_speed = agv_half_speed;}
//	else if(sensor_front > mtr_0p5){obstacles_front = 4; agv_speed = agv_full_speed;}
//	else if(sensor_front > mtr_1p0){obstacles_front = 3; agv_speed = agv_half_speed;}
//	else if(sensor_front > mtr_1p5) obstacles_front = 2;
//	else if(sensor_front > mtr_2p0) obstacles_front = 1;
//	else if(sensor_front > mtr_open)obstacles_front = 0;
	
	if(sensor_front >= mtr_stop) 	 	obstacles_front = 5;
	else if(sensor_front > mtr_0p5)	obstacles_front = 4;
	else if(sensor_front > mtr_1p0)	obstacles_front = 3;
	else if(sensor_front > mtr_1p5) obstacles_front = 2;
	else if(sensor_front > mtr_2p0) obstacles_front = 1;
	else if(sensor_front > mtr_open)obstacles_front = 0;
	
//	switch (obstacles_front){
//		case 5 :
//			
//			if(motor_status != servo_dir_stop){
//				agv_moving_speed = 0;
//				agv_speed = 0;
//			}
//			break;
//		case 4 :
//			
//			if(motor_status != servo_dir_stop){
//				agv_moving_speed = 0;
//				agv_speed = 0;
//			}
//			break;
//		case 3 :
//			if(motor_status != servo_dir_stop){
//				agv_moving_speed = agv_half_speed;
//				agv_speed = agv_half_speed;
//			}
//			break;
//		case 2 :
//			if(motor_status != servo_dir_stop){
//				agv_moving_speed = agv_full_speed;
//				agv_speed = agv_full_speed;
//			}
//			break;
//		case 1 :
//			if(motor_status != servo_dir_stop){
//				agv_moving_speed = agv_full_speed;
//				agv_speed = agv_full_speed;
//			}
//			break;
//	}
	
	if(sensor_back >= mtr_stop) 		obstacles_back = 5;
	else if(sensor_back > mtr_0p5) 	obstacles_back = 4;
	else if(sensor_back > mtr_1p0) 	obstacles_back = 3;
	else if(sensor_back > mtr_1p5) 	obstacles_back = 2;
	else if(sensor_back > mtr_2p0) 	obstacles_back = 1;
	else if(sensor_back > mtr_open)	obstacles_back = 0;
	
	busy_check(busy_reset);
}
