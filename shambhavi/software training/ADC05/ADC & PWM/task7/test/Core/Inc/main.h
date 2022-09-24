/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TRACE_CK_Pin GPIO_PIN_2
#define TRACE_CK_GPIO_Port GPIOE
#define TRACE_D0_Pin GPIO_PIN_3
#define TRACE_D0_GPIO_Port GPIOE
#define TRACE_D1_Pin GPIO_PIN_4
#define TRACE_D1_GPIO_Port GPIOE
#define TRACE_D2_Pin GPIO_PIN_5
#define TRACE_D2_GPIO_Port GPIOE
#define Key_Button_Pin GPIO_PIN_6
#define Key_Button_GPIO_Port GPIOE
#define JOY_SEL_Pin GPIO_PIN_13
#define JOY_SEL_GPIO_Port GPIOC
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define SPI2_SCK_Pin GPIO_PIN_9
#define SPI2_SCK_GPIO_Port GPIOF
#define PF0_OSC_IN_Pin GPIO_PIN_0
#define PF0_OSC_IN_GPIO_Port GPIOF
#define PF1_OSC_OUT_Pin GPIO_PIN_1
#define PF1_OSC_OUT_GPIO_Port GPIOF
#define COM_IN__Pin GPIO_PIN_1
#define COM_IN__GPIO_Port GPIOC
#define LDR_OUT_Pin GPIO_PIN_0
#define LDR_OUT_GPIO_Port GPIOA
#define OPAMP1_IN__Pin GPIO_PIN_1
#define OPAMP1_IN__GPIO_Port GPIOA
#define OPAMP1_OUT_Pin GPIO_PIN_2
#define OPAMP1_OUT_GPIO_Port GPIOA
#define OPAMP1_IN_A3_Pin GPIO_PIN_3
#define OPAMP1_IN_A3_GPIO_Port GPIOA
#define I2S_WS_Pin GPIO_PIN_4
#define I2S_WS_GPIO_Port GPIOA
#define MC_DAC_OUT2_Pin GPIO_PIN_5
#define MC_DAC_OUT2_GPIO_Port GPIOA
#define OPAMP2_OUT_Pin GPIO_PIN_6
#define OPAMP2_OUT_GPIO_Port GPIOA
#define OPAMP2_IN__Pin GPIO_PIN_7
#define OPAMP2_IN__GPIO_Port GPIOA
#define USART1_TX_Pin GPIO_PIN_4
#define USART1_TX_GPIO_Port GPIOC
#define OPAMP2_IN_C5_Pin GPIO_PIN_5
#define OPAMP2_IN_C5_GPIO_Port GPIOC
#define Hum_In_Pin GPIO_PIN_0
#define Hum_In_GPIO_Port GPIOB
#define OPAMP3_OUT_Pin GPIO_PIN_1
#define OPAMP3_OUT_GPIO_Port GPIOB
#define Pin_1V8_POR_Pin GPIO_PIN_2
#define Pin_1V8_POR_GPIO_Port GPIOB
#define JOY_UP_Pin GPIO_PIN_7
#define JOY_UP_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_11
#define LED4_GPIO_Port GPIOE
#define LED_DRV_Pin GPIO_PIN_13
#define LED_DRV_GPIO_Port GPIOE
#define SPI2_DIR_Pin GPIO_PIN_14
#define SPI2_DIR_GPIO_Port GPIOE
#define MicroSD_CS_Pin GPIO_PIN_15
#define MicroSD_CS_GPIO_Port GPIOE
#define OPAMP4_IN__Pin GPIO_PIN_10
#define OPAMP4_IN__GPIO_Port GPIOB
#define SHIELD_CT_Pin GPIO_PIN_11
#define SHIELD_CT_GPIO_Port GPIOB
#define SHIELD_Pin GPIO_PIN_12
#define SHIELD_GPIO_Port GPIOB
#define OPAMP4_IN2__Pin GPIO_PIN_13
#define OPAMP4_IN2__GPIO_Port GPIOB
#define SPI2_MISO_Pin GPIO_PIN_14
#define SPI2_MISO_GPIO_Port GPIOB
#define SPI2_MOSI_Pin GPIO_PIN_15
#define SPI2_MOSI_GPIO_Port GPIOB
#define TS_CT_Pin GPIO_PIN_12
#define TS_CT_GPIO_Port GPIOD
#define TS1_Pin GPIO_PIN_13
#define TS1_GPIO_Port GPIOD
#define LED_FB_Pin GPIO_PIN_14
#define LED_FB_GPIO_Port GPIOD
#define TS2_Pin GPIO_PIN_15
#define TS2_GPIO_Port GPIOD
#define SDCard_Detect_Pin GPIO_PIN_6
#define SDCard_Detect_GPIO_Port GPIOC
#define Hum_Out_Pin GPIO_PIN_8
#define Hum_Out_GPIO_Port GPIOC
#define I2S_CKIN_Pin GPIO_PIN_9
#define I2S_CKIN_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_8
#define MCO_GPIO_Port GPIOA
#define I2S_MCK_Pin GPIO_PIN_9
#define I2S_MCK_GPIO_Port GPIOA
#define I2C2_SDA_Pin GPIO_PIN_10
#define I2C2_SDA_GPIO_Port GPIOA
#define TMS_SWDIO_Pin GPIO_PIN_13
#define TMS_SWDIO_GPIO_Port GPIOA
#define I2C2_SCL_Pin GPIO_PIN_6
#define I2C2_SCL_GPIO_Port GPIOF
#define TMS_SWCLK_Pin GPIO_PIN_14
#define TMS_SWCLK_GPIO_Port GPIOA
#define TDI_Pin GPIO_PIN_15
#define TDI_GPIO_Port GPIOA
#define I2S_SCK_Pin GPIO_PIN_10
#define I2S_SCK_GPIO_Port GPIOC
#define I2S_DOUT_Pin GPIO_PIN_11
#define I2S_DOUT_GPIO_Port GPIOC
#define I2S_SDIN_Pin GPIO_PIN_12
#define I2S_SDIN_GPIO_Port GPIOC
#define CAN_RX_Pin GPIO_PIN_0
#define CAN_RX_GPIO_Port GPIOD
#define CAN_TX_Pin GPIO_PIN_1
#define CAN_TX_GPIO_Port GPIOD
#define JOY_DOWN_Pin GPIO_PIN_5
#define JOY_DOWN_GPIO_Port GPIOD
#define JOY_RIGHT_Pin GPIO_PIN_6
#define JOY_RIGHT_GPIO_Port GPIOD
#define E2P_CS_Pin GPIO_PIN_7
#define E2P_CS_GPIO_Port GPIOD
#define TDO_SWO_Pin GPIO_PIN_3
#define TDO_SWO_GPIO_Port GPIOB
#define TRST_Pin GPIO_PIN_4
#define TRST_GPIO_Port GPIOB
#define JOY_LEFT_Pin GPIO_PIN_5
#define JOY_LEFT_GPIO_Port GPIOB
#define USB_Disconnect_Pin GPIO_PIN_8
#define USB_Disconnect_GPIO_Port GPIOB
#define IR_LED_Pin GPIO_PIN_9
#define IR_LED_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOE
#define USART1_RX_Pin GPIO_PIN_1
#define USART1_RX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
