/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

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
#define PWR_GD_Pin GPIO_PIN_12
#define PWR_GD_GPIO_Port GPIOC
#define PWR_GD_EXTI_IRQn EXTI15_10_IRQn
#define TS_Pin GPIO_PIN_11
#define TS_GPIO_Port GPIOA
#define EOC_Pin GPIO_PIN_6
#define EOC_GPIO_Port GPIOC
#define EOC_EXTI_IRQn EXTI9_5_IRQn
#define Lightining_intr_Pin GPIO_PIN_0
#define Lightining_intr_GPIO_Port GPIOB
#define Lightining_intr_EXTI_IRQn EXTI0_IRQn
#define GRN_Pin GPIO_PIN_14
#define GRN_GPIO_Port GPIOD
#define YEL_Pin GPIO_PIN_13
#define YEL_GPIO_Port GPIOD
#define WHT_Pin GPIO_PIN_12
#define WHT_GPIO_Port GPIOD
#define RED_Pin GPIO_PIN_15
#define RED_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
