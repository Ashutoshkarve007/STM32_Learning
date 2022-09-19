#include "tim.h"


#define mtr_stop  0xFD
#define mtr_0p3   0xFC
#define mtr_0p4   0xFB
#define mtr_0p5   0xFA
#define mtr_0p6   0xF9
#define mtr_0p7   0xF8
#define mtr_0p8   0xF7
#define mtr_0p9   0xF6
#define mtr_1p0   0xF5
#define mtr_1p1   0xF4
#define mtr_1p2   0xF3
#define mtr_1p3   0xF2
#define mtr_1p4   0xF1
#define mtr_1p5   0xF0
#define mtr_1p6   0xEF
#define mtr_1p7   0xEE
#define mtr_1p8   0xED
#define mtr_1p9   0xEC
#define mtr_2p0   0xEB
#define mtr_open  0xE5

#define obstacle_stop	5
#define obstacle_0p5	4
#define obstacle_1p0	3
#define obstacle_1p5	2
#define obstacle_2p0	0
#define obstacle_open	0

#define obstacle_rgb_color_mtr_open	RGB_CYAN
#define obstacle_rgb_color_mtr_1p5	RGB_CYAN
#define obstacle_rgb_color_mtr_1p0	RGB_CYAN
#define obstacle_rgb_color_mtr_0p5	RGB_GREEN
#define obstacle_rgb_color_mtr_0p1	RGB_RED

//#define obstacle_rgb_color_mtr_open	RGB_CYAN
//#define obstacle_rgb_color_mtr_1p5	0xFFA500
//#define obstacle_rgb_color_mtr_1p0	0xFFFF00
//#define obstacle_rgb_color_mtr_0p5	0xFF0000

void obstacle_sensor_init(void);
void obstacle_check(void);
