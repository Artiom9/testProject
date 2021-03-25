/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define Data_01_Pin GPIO_PIN_0
#define Data_01_GPIO_Port GPIOC
#define Data_01_EXTI_IRQn EXTI0_IRQn
#define Data_11_Pin GPIO_PIN_1
#define Data_11_GPIO_Port GPIOC
#define Data_11_EXTI_IRQn EXTI1_IRQn
#define BT_0_Pin GPIO_PIN_2
#define BT_0_GPIO_Port GPIOC
#define BT_0_EXTI_IRQn EXTI2_IRQn
#define BT_1_Pin GPIO_PIN_3
#define BT_1_GPIO_Port GPIOC
#define BT_1_EXTI_IRQn EXTI3_IRQn
#define IN_00_Pin GPIO_PIN_4
#define IN_00_GPIO_Port GPIOC
#define IN_00_EXTI_IRQn EXTI4_IRQn
#define Zumer1_Pin GPIO_PIN_10
#define Zumer1_GPIO_Port GPIOB
#define Zumer2_Pin GPIO_PIN_11
#define Zumer2_GPIO_Port GPIOB
#define GreenLed_1_Pin GPIO_PIN_14
#define GreenLed_1_GPIO_Port GPIOB
#define GreenLed_2_Pin GPIO_PIN_15
#define GreenLed_2_GPIO_Port GPIOB
#define Data_02_Pin GPIO_PIN_6
#define Data_02_GPIO_Port GPIOC
#define Data_02_EXTI_IRQn EXTI9_5_IRQn
#define Data_12_Pin GPIO_PIN_7
#define Data_12_GPIO_Port GPIOC
#define Data_12_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
