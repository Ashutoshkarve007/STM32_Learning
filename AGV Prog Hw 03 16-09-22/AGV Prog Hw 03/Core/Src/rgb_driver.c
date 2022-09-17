#include "main.h"
#include "schedule.h"
#include "rgb_driver.h"
#include "obstacle_sensor.h"

unsigned long rgb_color_array[no_of_rgb_led];
uint32_t rgb_bit_duty_cycle_array[rgb_array_size];  // Array To save RGB Byte PWM values

extern unsigned long global_color;
extern uint8_t obstacles_front,obstacles_back;
extern uint8_t ms;

extern uint8_t rgb_pattern,rgb_pattern_prev;
extern uint8_t agv_moving_dir;
extern bool rgb_blink_flag;

void rgb_upload(){
	busy_check(busy_set);
	
	
	/* Variable For Color  */
	unsigned long color;
	
	/* Variable For Brightness*/
	uint8_t brightness;
	
	rgb_bit_duty_cycle_array[0] = 0;
	
	for(int rgbLedNo = 0 ; rgbLedNo < no_of_rgb_led; rgbLedNo ++){
		color = rgb_color_array[rgbLedNo] & 0xFFFFFF;
		brightness = rgb_color_array[rgbLedNo] >> 24;
		
		//color 			= global_color;
		//brightness 	= 100;
		
		unsigned char *p = (unsigned char*)&color;
		unsigned int r = (p[2] *brightness)/100;
		unsigned int g = (p[1] *brightness)/100;
		unsigned int b = (p[0] *brightness)/100;
		
		uint32_t addr_g = 1 + (24 * rgbLedNo);
		uint32_t addr_r = 8 + addr_g;
		uint32_t addr_b = 16+ addr_g;
		
		for(uint8_t shift = 0x80;shift>0;shift = shift >> 1)
		{
			if(shift & r)			rgb_bit_duty_cycle_array[addr_r]= 58;
			else							rgb_bit_duty_cycle_array[addr_r]= 29;

			if(shift & g)			rgb_bit_duty_cycle_array[addr_g]= 58;
			else							rgb_bit_duty_cycle_array[addr_g]= 29;

			if(shift & b)			rgb_bit_duty_cycle_array[addr_b]= 58;
			else							rgb_bit_duty_cycle_array[addr_b]= 29;

			addr_r++; addr_g++; addr_b++;
		}
		
	}
	HAL_TIM_PWM_Start_DMA(&htim15, TIM_CHANNEL_1, rgb_bit_duty_cycle_array,rgb_array_size);
	busy_check(busy_reset);
}

void rgb_clear(){
	busy_check(busy_set);
	unsigned long rgb_clear_data = (brightness_value << 24) | RGB_BLACK;
	for(int rgbLedNo = 0 ; rgbLedNo < no_of_rgb_led; rgbLedNo ++) rgb_color_array[rgbLedNo] =  rgb_clear_data;
	rgb_upload();
	busy_check(busy_reset);
}

void rgb_comet_load(){
	busy_check(busy_set);
	static int comet_rgbLedNo[4] = {0,0,0,0};
	unsigned long comet_rgb_color;  //(brightness_value << 24) | 
	
	switch(agv_moving_dir){
		case AGV_dir_forward:
		//case AGV_dir_stop:	
			
			comet_rgb_color = RGB_BLACK;
			
			rgb_color_array[comet_rgbLedNo[0]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[1]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[2]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[3]] =  	comet_rgb_color;
		
//			if(comet_rgbLedNo[0] < no_of_rgb_led) 	comet_rgbLedNo[0]++;
//			else																		comet_rgbLedNo[0] = 0;
		
			/* Right Side Movement */
			if(comet_rgbLedNo[0] < rgb_right_end) 	comet_rgbLedNo[0]++;
			else																		comet_rgbLedNo[0] = rgb_right_start;	
			
			/* left Side Movement */
			if(comet_rgbLedNo[2] <= rgb_left_start)	comet_rgbLedNo[2] = rgb_left_end;
			else																		comet_rgbLedNo[2]--;
			
			
			comet_rgb_color = RGB_BLACK;
				
			switch(obstacles_front){
				case obstacle_open:
					if(ms > 5) 	comet_rgb_color = (brightness_value << 24) | obstacle_rgb_color_mtr_open;
					break;
				case obstacle_1p5:
					if(ms > 5) 	comet_rgb_color = (brightness_value << 24) | obstacle_rgb_color_mtr_1p5;
					break;
				case obstacle_1p0:
					if(ms > 5) 	comet_rgb_color = (brightness_value << 24) | obstacle_rgb_color_mtr_0p5;
					break;
				case obstacle_0p5:
					if(ms > 5) 	comet_rgb_color = (brightness_value << 24) | obstacle_rgb_color_mtr_0p1;
					break;
			}
			
			for(uint16_t i = rgb_front_start; i <= rgb_front_end; i++){
				rgb_color_array[i] =  comet_rgb_color;			
				
				
				//rgb_color_array[i] = 
			}
		
			comet_rgb_color = (brightness_value << 24) | global_color;
			rgb_color_array[comet_rgbLedNo[0]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[1]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[2]] =  	comet_rgb_color;
			rgb_color_array[comet_rgbLedNo[3]] =  	comet_rgb_color;
			
		
			break;
			
			
		case AGV_dir_stop:
			for(int rgbLedNo = 0 ; rgbLedNo < no_of_rgb_led; rgbLedNo ++) rgb_color_array[rgbLedNo] =  0x0;
			break;	
		
	}
	
	rgb_upload();
	busy_check(busy_reset);
}
void rgb_full_on(){
	busy_check(busy_set);
	
	
	unsigned long full_rgb_color;
	full_rgb_color = (brightness_value << 24) | global_color;
	for(int rgbLedNo = 0 ; rgbLedNo < no_of_rgb_led; rgbLedNo ++){
		rgb_color_array[rgbLedNo] = full_rgb_color;
	}
	rgb_upload();
	
	busy_check(busy_reset);
}
void rgb_comet_load1(){
	busy_check(busy_set);
	static int comet_rgbLedNo[6] = {0,10,20,30,40,50};
	unsigned long comet_rgb_color;  //(brightness_value << 24) | 
	
	uint8_t i = 0;
	
	for( i = 0 ; i < 6 ; i++)
	{
		comet_rgb_color = RGB_BLACK;
		rgb_color_array[comet_rgbLedNo[i]] =  	comet_rgb_color;
		
		if(comet_rgbLedNo[i] < no_of_rgb_led) 	comet_rgbLedNo[i]++;
		else																comet_rgbLedNo[i] = 0;	
		
		comet_rgb_color = (brightness_value << 24) | global_color;
		rgb_color_array[comet_rgbLedNo[i]] =  	comet_rgb_color;
	}
	rgb_upload();
	busy_check(busy_reset);
}

void rgb_check(){
	busy_check(busy_set);
	
	if( (rgb_pattern == rgb_stop) && (rgb_pattern != rgb_pattern_prev) ) 	rgb_clear();
	if(	rgb_pattern  == rgb_comet ) 																			rgb_comet_load();
	if(	rgb_pattern  == rgb_full )																				rgb_full_on();
		
	rgb_pattern_prev = rgb_pattern;
	busy_check(busy_reset);
}
