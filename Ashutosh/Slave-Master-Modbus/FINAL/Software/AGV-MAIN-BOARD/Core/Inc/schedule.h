#include "main.h"
#include "tim.h"

#define busy_set 		true
#define busy_reset 	false
	
#define ms_count		10

#define agv_full_speed  	10
#define agv_half_speed  	5
#define agv_start_speed  	1
#define agv_stop_speed  	0

#define AGV_dir_stop			0
#define AGV_dir_forward		2
#define AGV_dir_back			3
#define AGV_dir_left			4
#define AGV_dir_right			5

void agv_clk_start(void);
void agv_clk_stop(void);

void busy_check(bool);
void schedule(void);
void switch_control(void);

