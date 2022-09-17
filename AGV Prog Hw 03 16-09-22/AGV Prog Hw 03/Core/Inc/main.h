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
#include "stdbool.h"
#include "schedule.h"
#include "servo_modbus.h"
#include "rgb_driver.h"
#include "obstacle_sensor.h"
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Busy_Pin GPIO_PIN_13
#define Busy_GPIO_Port GPIOC
#define RGB_Pin GPIO_PIN_9
#define RGB_GPIO_Port GPIOF
#define Bat_volt_Pin GPIO_PIN_1
#define Bat_volt_GPIO_Port GPIOA
#define ESP_SS_Pin GPIO_PIN_4
#define ESP_SS_GPIO_Port GPIOA
#define ESP_SCK_Pin GPIO_PIN_5
#define ESP_SCK_GPIO_Port GPIOA
#define ESP_MISO_Pin GPIO_PIN_6
#define ESP_MISO_GPIO_Port GPIOA
#define ESP_MOSI_Pin GPIO_PIN_7
#define ESP_MOSI_GPIO_Port GPIOA
#define ESP_INT_Pin GPIO_PIN_4
#define ESP_INT_GPIO_Port GPIOC
#define ESP_RST_Pin GPIO_PIN_5
#define ESP_RST_GPIO_Port GPIOC
#define M_Sw_1_Pin GPIO_PIN_0
#define M_Sw_1_GPIO_Port GPIOB
#define M_Sw_2_Pin GPIO_PIN_1
#define M_Sw_2_GPIO_Port GPIOB
#define M_Sw_3_Pin GPIO_PIN_2
#define M_Sw_3_GPIO_Port GPIOB
#define M_Sw_4_Pin GPIO_PIN_7
#define M_Sw_4_GPIO_Port GPIOE
#define LED1_AGV_CLK_Pin GPIO_PIN_8
#define LED1_AGV_CLK_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_11
#define LED4_GPIO_Port GPIOE
#define Remote_1_Pin GPIO_PIN_14
#define Remote_1_GPIO_Port GPIOE
#define Remote_1_EXTI_IRQn EXTI15_10_IRQn
#define Remote_2_Pin GPIO_PIN_15
#define Remote_2_GPIO_Port GPIOE
#define Remote_2_EXTI_IRQn EXTI15_10_IRQn
#define Remote_Lf_Pin GPIO_PIN_10
#define Remote_Lf_GPIO_Port GPIOB
#define Remote_Lf_EXTI_IRQn EXTI15_10_IRQn
#define Remote_Rt_Pin GPIO_PIN_11
#define Remote_Rt_GPIO_Port GPIOB
#define Remote_Rt_EXTI_IRQn EXTI15_10_IRQn
#define Remote_Fr_Pin GPIO_PIN_12
#define Remote_Fr_GPIO_Port GPIOB
#define Remote_Fr_EXTI_IRQn EXTI15_10_IRQn
#define Remote_Bk_Pin GPIO_PIN_13
#define Remote_Bk_GPIO_Port GPIOB
#define Remote_Bk_EXTI_IRQn EXTI15_10_IRQn
#define SW_3_Pin GPIO_PIN_14
#define SW_3_GPIO_Port GPIOB
#define SW_4_Pin GPIO_PIN_15
#define SW_4_GPIO_Port GPIOB
#define HX711_DO_Pin GPIO_PIN_8
#define HX711_DO_GPIO_Port GPIOC
#define HX711_SCK_Pin GPIO_PIN_9
#define HX711_SCK_GPIO_Port GPIOC
#define HMI_RX_Pin GPIO_PIN_9
#define HMI_RX_GPIO_Port GPIOA
#define HMI_TX_Pin GPIO_PIN_10
#define HMI_TX_GPIO_Port GPIOA
#define MAX3485_2_DE_Pin GPIO_PIN_15
#define MAX3485_2_DE_GPIO_Port GPIOA
#define MAX3485_2_DI_Pin GPIO_PIN_10
#define MAX3485_2_DI_GPIO_Port GPIOC
#define MAX3485_2_RO_Pin GPIO_PIN_11
#define MAX3485_2_RO_GPIO_Port GPIOC
#define Servo_modbus_DE_Pin GPIO_PIN_4
#define Servo_modbus_DE_GPIO_Port GPIOD
#define Servo_modbus_DI_Pin GPIO_PIN_5
#define Servo_modbus_DI_GPIO_Port GPIOD
#define Servo_modbus_RO_Pin GPIO_PIN_6
#define Servo_modbus_RO_GPIO_Port GPIOD
#define Obstacle_Sensor_Pin GPIO_PIN_4
#define Obstacle_Sensor_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
