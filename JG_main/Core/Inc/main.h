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
#define W2_CW_PWM_Pin GPIO_PIN_0
#define W2_CW_PWM_GPIO_Port GPIOB
#define W2_CCW_PWM_Pin GPIO_PIN_1
#define W2_CCW_PWM_GPIO_Port GPIOB
#define W3_CCW_SIG_Pin GPIO_PIN_13
#define W3_CCW_SIG_GPIO_Port GPIOB
#define W3_CCW_SIG_EXTI_IRQn EXTI15_10_IRQn
#define W4_CCW_SIG_Pin GPIO_PIN_14
#define W4_CCW_SIG_GPIO_Port GPIOB
#define W4_CCW_SIG_EXTI_IRQn EXTI15_10_IRQn
#define W2_CW_SIG_Pin GPIO_PIN_15
#define W2_CW_SIG_GPIO_Port GPIOB
#define W2_CW_SIG_EXTI_IRQn EXTI15_10_IRQn
#define W1_CCW_SIG_Pin GPIO_PIN_8
#define W1_CCW_SIG_GPIO_Port GPIOA
#define W1_CCW_SIG_EXTI_IRQn EXTI9_5_IRQn
#define W3_CCW_PWM_Pin GPIO_PIN_9
#define W3_CCW_PWM_GPIO_Port GPIOA
#define W3_CW_PWM_Pin GPIO_PIN_10
#define W3_CW_PWM_GPIO_Port GPIOA
#define W3_CW_SIG_Pin GPIO_PIN_11
#define W3_CW_SIG_GPIO_Port GPIOA
#define W3_CW_SIG_EXTI_IRQn EXTI15_10_IRQn
#define W4_CW_SIG_Pin GPIO_PIN_12
#define W4_CW_SIG_GPIO_Port GPIOA
#define W4_CW_SIG_EXTI_IRQn EXTI15_10_IRQn
#define LEFT_BUMP_Pin GPIO_PIN_3
#define LEFT_BUMP_GPIO_Port GPIOB
#define LEFT_BUMP_EXTI_IRQn EXTI3_IRQn
#define RIGHT_BUMP_Pin GPIO_PIN_4
#define RIGHT_BUMP_GPIO_Port GPIOB
#define W1_CCW_PWM_Pin GPIO_PIN_6
#define W1_CCW_PWM_GPIO_Port GPIOB
#define W1_CW_PWM_Pin GPIO_PIN_7
#define W1_CW_PWM_GPIO_Port GPIOB
#define W4_CW_PWM_Pin GPIO_PIN_8
#define W4_CW_PWM_GPIO_Port GPIOB
#define W4_CCW_PWM_Pin GPIO_PIN_9
#define W4_CCW_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
