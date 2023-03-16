#include "main.h"

#define FLASH_Switch1_Page   ((uint32_t)0x08006000) /* Base address of Page 120, 2 Kbytes */
#define FLASH_Switch2_Page   ((uint32_t)0x08006800) /* Base address of Page 121, 2 Kbytes */
#define FLASH_Switch3_Page   ((uint32_t)0x08007000) /* Base address of Page 122, 2 Kbytes */
#define FLASH_Switch4_Page   ((uint32_t)0x08007800) /* Base address of Page 123, 2 Kbytes */
#define FLASH_Switch5_Page   ((uint32_t)0x08008000) /* Base address of Page 124, 2 Kbytes */
#define FLASH_Switch6_Page   ((uint32_t)0x08008800) /* Base address of Page 125, 2 Kbytes */

uint8_t page_erase(uint8_t sw);
uint32_t switch_page_address(uint8_t sw);
uint32_t next_page_of(uint8_t sw);
void page_load(uint8_t sw);
uint32_t schedule_save_of_switch(uint8_t sw,uint32_t address,uint32_t data);