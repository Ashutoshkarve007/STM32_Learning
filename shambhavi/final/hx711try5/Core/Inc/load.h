#include "main.h"
#include "tim.h"

#define   _HX711_DELAY_US_LOOP  4

//#define Weight_Lode
//#define HX711_Buffer

void hx711_delay_us(void);
unsigned long hx711_read(void);
void Get_Weight(void);