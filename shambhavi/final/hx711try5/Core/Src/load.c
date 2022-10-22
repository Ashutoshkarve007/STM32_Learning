#include "load.h"  //you have to add this header file in main.h

unsigned long Weight_No_Lode = 32279 ; //it is the value of count which we recieve in debug mode without load
unsigned long  HX711_Buffer; // the avg value of 10 values of count
unsigned long Weight_Lode;  //it is the value of count which we recieve in debug mode with load
unsigned long Weight_Real;  //real weight after calibration(we are calibrating with known load)
unsigned long Kp_Weight = 43; //it is calibrating by the known weight of 1kg,2kg,3kg take the avg value

	uint8_t i;
	uint32_t count = 0;


 void delay_us (uint16_t us) //it is for generating microsecond delay using with timer
  {
	  __HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	  while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
  }

 unsigned long hx711_read() // function for read data from hx711
	{
		 HAL_GPIO_WritePin(clock_GPIO_Port,clock_Pin, GPIO_PIN_RESET); //make clock pin low first
	
	   HAL_GPIO_TogglePin(led1_GPIO_Port,led1_Pin); //just for check
	
		 delay_us(1); //using from microsecond delay function which we define in load.c

		 count = 0;	//SET count inhitially zero
	
		for(i=0;i<24;i++)  //it is a 24 bit ADC that's why we give a counting to 24
		{
			HAL_GPIO_WritePin(clock_GPIO_Port,clock_Pin,GPIO_PIN_SET);
			
			count = count<< 1; //shift the count from 1 after the seting the clock high
			delay_us(11);
			
			HAL_GPIO_WritePin(clock_GPIO_Port,clock_Pin,GPIO_PIN_RESET);
			
			if(HAL_GPIO_ReadPin(data_GPIO_Port,data_Pin)) 
				count++;                                   //if it reads data from data pin then increse the count from 
			delay_us(6);
		}
		  return count;
		
  }
	 //calibration part start from here

	void Get_Weight(void)   // for calibration to read function weight
  {
	  for(uint8_t j =1; j<=10; j++) //to collect avg value of weight from hx711_buffer, we take avg of 10 values
		 {
		  HX711_Buffer += count;  //add 10 values of hx711_buffer
		 }
	
	  Weight_Lode = HX711_Buffer/10;   //divide by 10 gor taking avg
		HX711_Buffer = 0;
		 
	  //Judgment is not empty
	  if(Weight_Lode> Weight_No_Lode)  
	    {
		    Weight_Real = (Weight_Lode-Weight_No_Lode)/Kp_Weight;  //formula for calibration
	    }
	  else if(Weight_Lode <= Weight_No_Lode)
		 Weight_Real = 0.0f;
	
	 //The serial port will display when the pulling force reaches a certain threshold
  //if(Weight_Real>100.0f)
 //printf("Current pull %u\r\n\r\n",Weight_Real);
}