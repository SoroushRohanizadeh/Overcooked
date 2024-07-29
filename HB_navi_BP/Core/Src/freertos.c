/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <usart.h>
#include "app_lineFollowing.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NUM_PINS 8
#define PERIOD 100
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
bool adcConvCMPLT = false;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
Rotary_Handle rotary_handle_w1 = {
  .countCW = 0,
  .countCCW = 0,
  .gpioPinCW = W1_CW_SIG_Pin,
  .gpioPinCCW = W1_CCW_SIG_Pin
};

Rotary_Handle rotary_handle_w2 = {
  .countCW = 0,
  .countCCW = 0,
  .gpioPinCW = W2_CW_SIG_Pin,
  .gpioPinCCW = W2_CCW_SIG_Pin
};

Rotary_Handle rotary_handle_w3 = {
  .countCW = 0,
  .countCCW = 0,
  .gpioPinCW = W3_CW_SIG_Pin,
  .gpioPinCCW = W3_CCW_SIG_Pin
};

Rotary_Handle rotary_handle_w4 = {
  .countCW = 0,
  .countCCW = 0,
  .gpioPinCW = W4_CW_SIG_Pin,
  .gpioPinCCW = W4_CCW_SIG_Pin
};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  UNUSED(hadc);
  adcConvCMPLT = true;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == rotary_handle_w1.gpioPinCW) {
    hw_rotaryEncoder_incrementCW(&rotary_handle_w1);
  } else if (GPIO_Pin == rotary_handle_w1.gpioPinCCW) {
    hw_rotaryEncoder_incrementCCW(&rotary_handle_w1);

  } else if (GPIO_Pin == rotary_handle_w2.gpioPinCW) {
    hw_rotaryEncoder_incrementCW(&rotary_handle_w2);

  } else if (GPIO_Pin == rotary_handle_w2.gpioPinCCW) {
    hw_rotaryEncoder_incrementCCW(&rotary_handle_w2);

  } else if (GPIO_Pin == rotary_handle_w3.gpioPinCW) {
    hw_rotaryEncoder_incrementCW(&rotary_handle_w3);

  } else if (GPIO_Pin == rotary_handle_w3.gpioPinCCW) {
    hw_rotaryEncoder_incrementCCW(&rotary_handle_w3);

  } else if (GPIO_Pin == rotary_handle_w4.gpioPinCW) {
    hw_rotaryEncoder_incrementCW(&rotary_handle_w4);

  } else if (GPIO_Pin == rotary_handle_w4.gpioPinCCW) {
    hw_rotaryEncoder_incrementCCW(&rotary_handle_w4);
  }
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  UNUSED(argument);

  volatile uint16_t adc_dma[NUM_PINS];

  // ADC_Pin adc_pins[] = {PA0, PA1};

  ADC_Handler adcHandler = {
    .hadcs = &hadc1,
    .twoADC = false,
    .numPins = NUM_PINS,
    .adcBuffer = adc_dma,
    .adcConvCMPLT = &adcConvCMPLT,
    // .adcPins = adc_pins,
    .ADC_Start = HAL_ADC_Start_DMA
  };

  PWM_Handle cw_pwmHandler_w1 = {
    .htim = &htim4,
    .channel = TIM_CHANNEL_2,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle ccw_pwmHandler_w1 = {
    .htim = &htim4,
    .channel = TIM_CHANNEL_1,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle cw_pwmHandler_w2 = {
    .htim = &htim3,
    .channel = TIM_CHANNEL_3,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle ccw_pwmHandler_w2 = {
    .htim = &htim3,
    .channel = TIM_CHANNEL_4,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle cw_pwmHandler_w3 = {
    .htim = &htim1,
    .channel = TIM_CHANNEL_2,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle ccw_pwmHandler_w3 = {
    .htim = &htim1,
    .channel = TIM_CHANNEL_3,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle cw_pwmHandler_w4 = {
    .htim = &htim4,
    .channel = TIM_CHANNEL_3,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle ccw_pwmHandler_w4 = {
    .htim = &htim4,
    .channel = TIM_CHANNEL_4,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  Motor_Handle wheel1 = {
    .cw_handle = &cw_pwmHandler_w1,
    .ccw_handle = &ccw_pwmHandler_w1,
    .state = MOTOR_STOP,
    .rotary_handle = &rotary_handle_w1,
    .pidIntegral = 0,
    .prevError = 0,
    .kP = 0.2,
    .kI = 1.0
  };

  Motor_Handle wheel2 = {
    .cw_handle = &cw_pwmHandler_w2,
    .ccw_handle = &ccw_pwmHandler_w2,
    .state = MOTOR_STOP,
    .rotary_handle = &rotary_handle_w2,
    .pidIntegral = 0,
    .prevError = 0,
    .kP = 0.2,
    .kI = 1.0
  };

  Motor_Handle wheel3 = {
    .cw_handle = &cw_pwmHandler_w3,
    .ccw_handle = &ccw_pwmHandler_w3,
    .state = MOTOR_STOP,
    .rotary_handle = &rotary_handle_w3,
    .pidIntegral = 0,
    .prevError = 0,
    .kP = 0.2,
    .kI = 1.0
  };

  Motor_Handle wheel4 = {
    .cw_handle = &cw_pwmHandler_w4,
    .ccw_handle = &ccw_pwmHandler_w4,
    .state = MOTOR_STOP,
    .rotary_handle = &rotary_handle_w4,
    .pidIntegral = 0,
    .prevError = 0,
    .kP = 0.2,
    .kI = 1.0
  };

  DT_Handle drive_handle = {
    .wheel_1 = &wheel1,
    .wheel_2 = &wheel2,
    .wheel_3 = &wheel3,
    .wheel_4 = &wheel4,
    .state = DRIVE_STOP
  };

  LF_Handle lf_handle = {
    .drive = &drive_handle,
    .sns = &adcHandler
  };

  uint8_t throt = 100;
  uint8_t throttle[] = {throt,throt,throt,throt};

  // app_drivetrain_drive(&drive_handle, throttle, RIGHT);

  hw_dcMotor_driveCCW(&wheel1, throt);

  int currentTicks = osKernelGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    // app_lineFollowing_tickPID(&lf_handle, throt, RIGHT);

    // app_drivetrain_tickThrottle(&drive_handle, throttle);

    hw_dcMotor_tickThrottlePID(&wheel1, throt);

    currentTicks += PERIOD;
    osDelayUntil(currentTicks);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

