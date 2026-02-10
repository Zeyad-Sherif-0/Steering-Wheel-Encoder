/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
/*============================= STANDARD INCLUDES =============================*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/*=============================================================================*/

/*============================= FREERTOS INCLUDES =============================*/
#include "FreeRTOS.h"
#include "task.h"
/*=============================================================================*/

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
#define ENC_0_Pin GPIO_PIN_0
#define ENC_0_GPIO_Port GPIOB
#define ENC_1_Pin GPIO_PIN_1
#define ENC_1_GPIO_Port GPIOB
#define ENC_2_Pin GPIO_PIN_2
#define ENC_2_GPIO_Port GPIOB
#define ENC_3_Pin GPIO_PIN_3
#define ENC_3_GPIO_Port GPIOB
#define ENC_4_Pin GPIO_PIN_4
#define ENC_4_GPIO_Port GPIOB
#define ENC_5_Pin GPIO_PIN_5
#define ENC_5_GPIO_Port GPIOB
#define ENC_6_Pin GPIO_PIN_6
#define ENC_6_GPIO_Port GPIOB
#define ENC_7_Pin GPIO_PIN_7
#define ENC_7_GPIO_Port GPIOB
#define ENC_8_Pin GPIO_PIN_8
#define ENC_8_GPIO_Port GPIOB
#define ENC_9_Pin GPIO_PIN_9
#define ENC_9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
