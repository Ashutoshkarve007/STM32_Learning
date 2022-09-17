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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DE_PIN_Pin GPIO_PIN_4
#define DE_PIN_GPIO_Port GPIOA
#define ID_PIN_1_Pin GPIO_PIN_12
#define ID_PIN_1_GPIO_Port GPIOB
#define ID_PIN_2_Pin GPIO_PIN_13
#define ID_PIN_2_GPIO_Port GPIOB
#define ID_PIN_3_Pin GPIO_PIN_14
#define ID_PIN_3_GPIO_Port GPIOB
#define ID_PIN_4_Pin GPIO_PIN_15
#define ID_PIN_4_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define RxBuf_SIZE 4                    //UART 2 data
#define MainBuf_SIZE 4                  //UART 2 data Size
#define RxSensorBuf_SIZE 10             //UART 1 data
#define MainSensorBuf_SIZE 9            //UART 1 data Size
#define slavedata_SIZE 7                //Final Sensor data Size from Slave
#define slaveerrordata_SIZE 5           //Error data Size from Slave
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
