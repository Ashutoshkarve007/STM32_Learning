/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define TFMINI_PIX_FLAG_END         0x0D0A     /*\r\n*/
#define TFMINI_PIX_FLAG_NEG         0x2D       /* - */
#define TFMINI_PIX_FLAG_DECPIONT    0x2E       /* . */

#define ASCII_0                     0x30       /*ASCII:0*/

//--------------<<< Use Configuration Wizard in Context Menu >>>---------------
//<h>   The Para Configuration
//      <o0> TFMINI_ACTION_DIST(0-65535, unit:cm)
//</h>
#define TFMINI_ACTION_DIST          200
//---------------<<< end of configuration section >>>--------------------------

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;

uint8_t g_usart1_rx_buf[USART_BUF_SIZE] = {0};    /*usart1_rx_buf*/
uint8_t g_usart2_rx_buf[USART_BUF_SIZE] = {0};    /*usart2_rx_buf*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart1, g_usart1_rx_buf, USART_BUF_SIZE);
  HAL_UART_Receive_DMA(&huart2, g_usart2_rx_buf, USART_BUF_SIZE);

  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //UART_IT_IDLE
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);    //UART_IT_IDLE

//  printf("TFMini\r\n");
 // printf("USART_2_DOUT\r\n");
 // printf("Argement: PIX\r\n");
 // printf("dist > %d cm, PA11 set Low;\r\ndist <= %d cm, PA8 set High.\r\n", TFMINI_ACTION_DIST, TFMINI_ACTION_DIST);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
  
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/*************************************************
Function: USART1_RX_Proc
Description: USART1_RX_Proc
Input:
    buf, data buffer
    len, data len
Output: None
Return: None
Others: TFMini output dist unit = cm;
*************************************************/
void USART1_RX_Proc(uint8_t *buf, uint32_t len)
{
    uint16_t cordist = 0;

    /*xxx.xx\r\n*/
    if((TFMINI_PIX_FLAG_END == (buf[len - 1] | (buf[len - 2] << 8))) \
        && (TFMINI_PIX_FLAG_DECPIONT == buf[len - 5]))
    {

        if(buf[0] == TFMINI_PIX_FLAG_NEG)   /*Negative, the amplitude value too low.*/
        {
            cordist = 1200;
        }
        else
        {
            cordist = ((buf[len - 6] - ASCII_0) * 100) + ((buf[len - 4] - ASCII_0) * 10) + (buf[len - 3] - ASCII_0);
            cordist+= (len == 7) ? ((buf[len - 7] - ASCII_0) * 1000) : 0;
        }

        /*cordist > TFMINI_ACTION_DIST cm, PA8 set Low;
          cordist <= TFMINI_ACTION_DIST cm, PA8 set High.*/
        if(HAL_GPIO_ReadPin(DOUT_GPIO_Port, DOUT_Pin) != GPIO_PIN_RESET)
        {
            if(cordist > TFMINI_ACTION_DIST)
            {
                HAL_GPIO_WritePin(DOUT_GPIO_Port, DOUT_Pin, GPIO_PIN_RESET);
							
            }
        }
        else
        {
            if(cordist <= TFMINI_ACTION_DIST)
            {
                HAL_GPIO_WritePin(DOUT_GPIO_Port, DOUT_Pin, GPIO_PIN_SET);
            }
        }
    }
}



/*************************************************
Function: USART2_RX_Proc
Description: USART2_RX_Proc
Input:
    buf, data buffer
    len, data len
Output: None
Return: None
Others:
*************************************************/
void USART2_RX_Proc(uint8_t *buf, uint32_t len)
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
    printf("_Error_Handler\r\nFile_Name:%s,line:%d",file, line);
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
