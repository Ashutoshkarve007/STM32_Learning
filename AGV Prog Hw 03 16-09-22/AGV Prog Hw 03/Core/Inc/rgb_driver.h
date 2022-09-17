#include "tim.h"

#define brightness_value  		10
#define no_of_rgb_led					132
#define rgb_array_size    		(no_of_rgb_led * 24) + 2

#define rgb_move_dir_For			1
#define rgb_move_dir_Rev			0

#define rgb_stop							0
#define rgb_full							1
#define rgb_comet							4

#define	rgb_ring							2

#define rgb_space							15
#define rgb_comet_nos					3

#define no_of_rgb_front				27
#define no_of_rgb_back				27
#define no_of_rgb_left				39
#define no_of_rgb_right				39


#define rgb_right_start				0
#define rgb_right_end					no_of_rgb_right	- 1
#define rgb_front_start				rgb_right_end 	+ 1
#define rgb_front_end					rgb_right_end		+	no_of_rgb_front
#define rgb_left_start				rgb_front_end 	+ 1
#define rgb_left_end					rgb_front_end		+	no_of_rgb_left
#define rgb_back_start				rgb_left_end		+ 1
#define rgb_back_end					rgb_left_end 		+ no_of_rgb_back

//#define rgb_back_start				0
//#define rgb_back_end					no_of_rgb_back 	- 1
//#define rgb_right_start				rgb_back_end 		+ 1
//#define rgb_right_end					rgb_back_end		+	no_of_rgb_right
//#define rgb_front_start				rgb_right_end 	+ 1
//#define rgb_front_end					rgb_right_end		+	no_of_rgb_front
//#define rgb_left_start				rgb_front_end 	+ 1
//#define rgb_left_end					rgb_front_end		+	no_of_rgb_left

#define RGB_WHITE					0xFFFFFF
#define RGB_SILVER				0xC0C0C0
#define RGB_GRAY					0x808080
#define RGB_BLACK					0x000000
#define RGB_RED						0xFF0000
#define RGB_MAROON				0x800000
#define RGB_YELLOW				0xFFFF00
#define RGB_OLIVE					0x808000
#define RGB_GREEN					0x008000	//00FF00		//0xFFFF00
#define RGB_AQUA					0x00FFFF
#define RGB_TEAL					0x008080
#define RGB_BLUE					0x008080	//0000FF	 	//0x008080
#define RGB_NAVY					0x000080
#define RGB_FUCHSIA				0xFF00FF	//grb
#define RGB_PURPLE				0x800080
#define RGB_INDGO   			0x4b0082
#define RGB_ORANGE  			0xFFA500
#define RGB_CYAN					0x00BCE3

/* RGB Connected Timers & Channels */
#define RGB_TMR						htim3
#define RGB_channel				TIM_CHANNEL_3


void rgb_upload(void);
void rgb_clear(void);
void rgb_comet_load(void);
void rgb_full_on(void);
void rgb_check(void);
