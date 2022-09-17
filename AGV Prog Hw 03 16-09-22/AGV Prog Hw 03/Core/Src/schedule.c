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
	
//	if(agv_moving_speed_dynamic > 0 && agv_moving_speed_dynamic < agv_full_speed ){
//		
//		servo_speed_change(servo_id_left,motor_status,agv_moving_speed_dynamic);
//		servo_speed_change(servo_id_right,motor_status,agv_moving_speed_dynamic);	
//		agv_moving_speed_dynamic++;
//	}	else if(motor_stop_del){
//		motor_stop_del--;
//		if(motor_stop_del == 0){
//			servo_control(servo_id_left ,servo_dir_stop,0);
//			servo_control(servo_id_right ,servo_dir_stop,0);
//			motor_status = servo_dir_stop;
//			agv_moving_speed_dynamic = 0;
//			agv_moving_dir = AGV_dir_stop;
//			motor_extra_stop_del = agv_full_speed;
//		} else {
//			servo_speed_change(servo_id_left,agv_moving_dir,motor_stop_del);
//			servo_speed_change(servo_id_right,agv_moving_dir,motor_stop_del);	
//		}
//	} 
//	
//	else if(motor_extra_stop_del){
//		motor_extra_stop_del--;
//		if(motor_extra_stop_del == 0){
//			servo_control(servo_id_left ,servo_dir_stop,0);
//			servo_control(servo_id_right ,servo_dir_stop,0);
//		}
//	}

	if(agv_moving_speed != agv_moving_speed_dynamic){
		if(agv_moving_speed > agv_moving_speed_dynamic){
			agv_moving_speed_dynamic++;
		} if(agv_moving_speed < agv_moving_speed_dynamic){
			agv_moving_speed_dynamic--;
		}
		servo_speed_change(servo_id_left, motor_status,agv_moving_speed_dynamic);
		servo_speed_change(servo_id_right,motor_status,agv_moving_speed_dynamic);
		
		if(agv_moving_speed == 0 && agv_moving_speed_dynamic == 0){
			servo_control(servo_id_left ,servo_dir_stop,0);
			servo_control(servo_id_right ,servo_dir_stop,0);
			motor_status = servo_dir_stop;
			agv_moving_dir = AGV_dir_stop;
			motor_extra_stop_del = agv_full_speed;
		}
	}
	else if(motor_extra_stop_del){
		motor_extra_stop_del--;
		if(motor_extra_stop_del == 0){
			servo_control(servo_id_left ,servo_dir_stop,0);
			servo_control(servo_id_right ,servo_dir_stop,0);
		}
	}
	
	//if(ms  == 1 || ms == 3 || ms  == 5 || ms == 7 || ms == 9) 
		switch_flag = true;
	//else
		rgb_check();
	
	if(ms == 2){
		//servo_control(servo_id_left ,servo_dir_back);
		//while(servo_dma_busy_flag)	HAL_Delay(1);
			//HAL_GPIO_TogglePin(Busy_GPIO_Port,Busy_Pin);
		
		//servo_control(servo_id_right,servo_dir_forw);
		
		// HAL_Delay(10);
		
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
		if(motor_status == servo_dir_stop){//(motor_status <= servo_dir_stop_p ){
		//if(motor_status != servo_dir_forw) {
			servo_control(servo_id_left,servo_dir_forw,1);
			servo_control(servo_id_right,servo_dir_forw,1);
			motor_status = servo_dir_forw; 
			motor_stop_del = 0; motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; //agv_moving_speed_dynamic = 1;
			agv_moving_dir = AGV_dir_forward;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_2_GPIO_Port,M_Sw_2_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Bk_GPIO_Port,Remote_Bk_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
		//if(motor_status != servo_dir_back) {
			servo_control(servo_id_left,servo_dir_back,1);
			servo_control(servo_id_right,servo_dir_back,1);
			motor_status = servo_dir_back; 
			motor_stop_del = 0; motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; //agv_moving_speed_dynamic = 1;
			agv_moving_dir = AGV_dir_back;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_3_GPIO_Port,M_Sw_3_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Lf_GPIO_Port,Remote_Lf_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
		//if(motor_status != servo_dir_left) {
			servo_control(servo_id_left,servo_dir_left,1);
			servo_control(servo_id_right,servo_dir_left,1);
			motor_status = servo_dir_left; 
			motor_stop_del = 0; motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; //agv_moving_speed_dynamic = 1;
			agv_moving_dir = AGV_dir_left;
		}
	}
	else if( HAL_GPIO_ReadPin(M_Sw_4_GPIO_Port,M_Sw_4_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Remote_Rt_GPIO_Port,Remote_Rt_Pin) == GPIO_PIN_RESET){
		if(motor_status == servo_dir_stop){
		//if(motor_status != servo_dir_right) {
			servo_control(servo_id_left,servo_dir_right,1);
			servo_control(servo_id_right,servo_dir_right,1);
			motor_status = servo_dir_right; 
			motor_stop_del = 0; motor_extra_stop_del = 0;
			agv_moving_speed = agv_speed; //agv_moving_speed_dynamic = 1;
			agv_moving_dir = AGV_dir_right;
		}
	}
	else if(motor_status > servo_dir_stop_p){
		//servo_control(servo_id_left ,servo_dir_stop,0);
		//servo_control(servo_id_right ,servo_dir_stop,0);
		//motor_status 		= servo_dir_stop_p;
		motor_stop_del 	= agv_full_speed;
		agv_moving_speed = 0;   //agv_moving_speed_dynamic = 0;
		//agv_moving_dir = AGV_dir_stop;
	
	}
	
//	if(motor_stop_del){
//		motor_stop_del--;
//		if(motor_stop_del == 0){
//			servo_control(servo_id_left ,servo_dir_stop,0);
//			servo_control(servo_id_right ,servo_dir_stop,0);
//			motor_status = servo_dir_stop;
//			agv_moving_speed_dynamic = 0;
//			agv_moving_dir = AGV_dir_stop;
//		} else {
//			servo_speed_change(servo_id_left,agv_moving_dir,motor_stop_del);
//			servo_speed_change(servo_id_right,agv_moving_dir,motor_stop_del);	
//		}
//		
//	}
	
	busy_check(busy_reset);
}

