#include "flash.h"

static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t Address = 0, PageError = 0;
uint8_t no_of_schedule=0;
long schedules[20];
uint32_t last_address=0;
uint32_t dat=0;


uint8_t page_erase(uint8_t sw)
{
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.NbPages     = 1;
	switch(sw)
	{
		case 1: 
			EraseInitStruct.PageAddress = FLASH_Switch1_Page;
			break;
		case 2: 
			EraseInitStruct.PageAddress = FLASH_Switch2_Page;
			break;
		case 3: 
			EraseInitStruct.PageAddress = FLASH_Switch3_Page;
			break;
		case 4: 
			EraseInitStruct.PageAddress = FLASH_Switch4_Page;
			break;
		case 5: 
			EraseInitStruct.PageAddress = FLASH_Switch5_Page;
			break;
		case 6: 
			EraseInitStruct.PageAddress = FLASH_Switch6_Page;
			break;
	}
	 
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)	return 0;
	else  																													return 1;		
}



uint32_t switch_page_address(uint8_t sw)
{
		switch(sw)
	{
		case 1: 
		  return FLASH_Switch1_Page;
			break;
		case 2: 
			 return FLASH_Switch2_Page;
			break;
		case 3: 
			 return FLASH_Switch3_Page;
			break;
		case 4: 
			 return FLASH_Switch4_Page;
			break;
		case 5: 
			 return FLASH_Switch5_Page;
			break;
		case 6: 
			 return FLASH_Switch6_Page;
			break;
	}
	
}


uint32_t next_page_of(uint8_t sw)
{
	return switch_page_address(sw+1);
	
}	

uint32_t last_shedule_address_of(uint8_t sw)
{
	return 0;

}

void page_load(uint8_t sw)
{
	
				//switch_page_address(sw);
			
				Address = switch_page_address(sw);
				for(int sh=0;sh<no_of_schedule;sh++)
				{
					    
							schedules[sh] = *(__IO uint32_t *)Address;
							Address = Address + 4;
							
												
				}
			
}
uint32_t schedule_save_of_switch(uint8_t sw,uint32_t address,uint32_t data)
{
	//HAL_FLASH_Unlock();
	//last_address=0;
	//address = switch_page_address(sw);
	//last_address=address+1;
	//while(address<last_address)
	//{
		//if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data)==HAL_OK)
		//address=address+1;
	//}
	/*if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data)==HAL_OK)
	{
		HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,1);
	}
	*/
	//HAL_FLASH_Lock();
	
	
	    uint32_t PAGEError = 0;
    uint32_t result = 0;

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area */
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = switch_page_address(sw); //User defined addr
    EraseInitStruct.NbPages     = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return 0;
    }

    /* Program the user Flash area word by word */
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return 0;
    }

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    /* Check if the programmed data is OK */
		
   	dat = data;
		//return x;
	
	
}
/*
uint32_t show_data_storein(uint32_t address)
{
	
	uint32_t x=*(__IO uint32_t *)address;
	return x;
}
*/