#include "schedule.h"

extern bool servo_dma_busy_flag;
bool ms_flag 				= false;
bool rgb_blink_flag = false;
bool switch_flag 		= false;

uint8_t motor_stop_del 				= 0;
uint8_t motor_extra_stop_del 	=	0;
uint8_t motor_status = servo_dir_stop;
uint8_t ms = 0;
uint8_t busy_count = 0;
uint8_t rgb_pattern 			= rgb_comet;
uint8_t rgb_pattern_prev 	= rgb_comet;

uint8_t agv_moving_dir						= AGV_dir_stop;
uint8_t agv_moving_speed					= 0;
uint8_t agv_moving_speed_dynamic	= 0;
uint8_t	agv_speed									= agv_full_speed;
uint8_t agv_remote_dir						= AGV_dir_stop;

uint8_t agv_left_moving_speed  = 0, agv_left_moving_speed_dynamic  = 0;
uint8_t agv_right_moving_speed = 0, agv_right_moving_speed_dynamic = 0;

void agv_clk_start(){
	HAL_TIM_Base_Start_IT(&htim6);
}

void agv_clk_stop(){
	HAL_TIM_Base_Stop_IT(&htim6);
}

void busy_check(bool busy){
	if(busy == busy_set){
		if(busy_count == 0) HAL_GPIO_WritePin(Busy_GPIO_Port,Busy_Pin,GPIO_PIN_SET);
		busy_count++;
	} else {
		if(busy_count) 		busy_count--;
		if(!busy_count) 	HAL_GPIO_WritePin(Busy_GPIO_Port,Busy_Pin,GPIO_PIN_RESET);
	}
}

void schedule(){
	//busy_check(busy_set);
	ms_flag = 0;
	
	ms++;
	
	
	if(ms > ms_count){
		ms = 0;
		
		//rgb_blink_flag = rgb_blink_flag;
	}
	
	/* Left Motor Speed adjust */
	if(agv_left_moving_speed != agv_left_moving_speed_dynamic){
		if(agv_left_moving_speed > agv_left_moving_speed_dynamic){
			agv_left_moving_speed_dynamic++;
		} if(agv_left_moving_speed < agv_left_moving_speed_dynamic){
			agv_left_moving_speed_dynamic--;
		}
		servo_speed_change(servo_id_left, motor_status,agv_left_moving_speed_dynamic);
		if(agv_left_moving_speed == agv_stop_speed && agv_left_moving_speed_dynamic == agv_stop_speed){
			servo_control(servo_id_left ,servo_dir_stop,agv_stop_speed);
			motor_status = servo_dir_stop;
			agv_moving_dir = AGV_dir_stop;
			motor_extra_stop_del = agv_full_speed;
		}
	}
	
	/* Right Motor Speed adjust */
	if(agv_right_moving_speed != agv_right_moving_speed_dynamic){
		if(agv_right_moving_speed > agv_right_moving_speed_dynamic){
			agv_right_moving_speed_dynamic++;
		} if(agv_right_moving_speed < agv_right_moving_speed_dynamic){
			agv_right_moving_speed_dynamic--;
		}
		servo_speed_change(servo_id_right, motor_status,agv_right_moving_speed_dynamic);
		if(agv_right_moving_speed == agv_stop_speed && agv_right_moving_speed_dynamic == agv_stop_speed){
			servo_control(servo_id_right ,servo_dir_stop,agv_stop_speed);
			motor_status = servo_dir_stop;
			agv_moving_dir = AGV_dir_stop;
			motor_extra_stop_del = agv_full_speed;
		}
	}
	
	/* Fail Safe stop code */
	if(motor_extra_stop_del){
		motor_extra_stop_del--;
		if(motor_extra_stop_del == agv_stop_speed){
			servo_control(servo_id_left ,servo_dir_stop,agv_stop_speed);
			servo_control(servo_id_right ,servo_dir_stop,agv_stop_speed);
		}
	}
	
	//if(ms  == 1 || ms == 3 || ms  == 5 || ms == 7 || ms == 9) 
		switch_flag = true;
	//else
		rgb_check();
	
	if(ms == 2){

		
	}
	/* To update RGB Status */
	//if(ms == 1) 
	
	//busy_check(busy_reset);
}

void switch_control(){
	busy_check(busy_set);
	
	switch_flag = false;
	agv_speed  = 10;
	
	if( HAL_GPIO_ReadPin(M_Sw_1_GPIO_Port,M_Sw_1_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Fr_GPIO_Port,Remote_Fr_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
			servo_control(servo_id_left,servo_dir_forw,agv_start_speed);
			servo_control(servo_id_right,servo_dir_forw,agv_start_speed);
			motor_status = servo_dir_forw; 
			motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; agv_left_moving_speed  = agv_speed; agv_right_moving_speed  = agv_speed; 
			agv_moving_dir = AGV_dir_forward;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_2_GPIO_Port,M_Sw_2_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Bk_GPIO_Port,Remote_Bk_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
			servo_control(servo_id_left,servo_dir_back,agv_start_speed);
			servo_control(servo_id_right,servo_dir_back,agv_start_speed);
			motor_status = servo_dir_back; 
			motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; agv_left_moving_speed  = agv_speed; agv_right_moving_speed  = agv_speed; 
			agv_moving_dir = AGV_dir_back;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_3_GPIO_Port,M_Sw_3_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Lf_GPIO_Port,Remote_Lf_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
			servo_control(servo_id_left,servo_dir_left,agv_start_speed);
			servo_control(servo_id_right,servo_dir_left,agv_start_speed);
			motor_status = servo_dir_left; 
			motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; agv_left_moving_speed  = agv_half_speed; agv_right_moving_speed  = agv_half_speed; 
			agv_moving_dir = AGV_dir_left;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_4_GPIO_Port,M_Sw_4_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Rt_GPIO_Port,Remote_Rt_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
			servo_control(servo_id_left,servo_dir_right,agv_start_speed);
			servo_control(servo_id_right,servo_dir_right,agv_start_speed);
			motor_status = servo_dir_right; 
			motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; agv_left_moving_speed  = agv_half_speed; agv_right_moving_speed  = agv_half_speed;  
			agv_moving_dir = AGV_dir_right;
		}
	}
	else if(motor_status > servo_dir_stop_p){
		agv_moving_speed = agv_stop_speed; agv_left_moving_speed  = agv_stop_speed; agv_right_moving_speed  = agv_stop_speed; 
	}

	busy_check(busy_reset);
}

