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
#include <hw_dcMotor.h>
#include <io_adc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <usart.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NUM_PINS 2
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
Rotary_Handle rotary_handle = {
  .countCW = 0,
  .countCCW = 0,
  .gpioPinCW = GPIO_PIN_15
};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  UNUSED(hadc);
  adcConvCMPLT = true;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin == rotary_handle.gpioPinCW) {
    hw_rotaryEncoder_incrementCW(&rotary_handle);
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

  // volatile uint16_t adc_dma[NUM_PINS];

  // ADC_Pin adc_pins[] = {PA0, PA1};

  // ADC_Handler adcHandler = {
  //   .hadcs = &hadc1,
  //   .twoADC = false,
  //   .numPins = NUM_PINS,
  //   .adcBuffer = adc_dma,
  //   .adcConvCMPLT = &adcConvCMPLT,
  //   .adcPins = adc_pins,
  //   .ADC_Start = HAL_ADC_Start_DMA
  // };

  PWM_Handle cw_pwmHandler = {
    .htim = &htim3,
    .channel = TIM_CHANNEL_3,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  PWM_Handle ccw_pwmHandler = {
    .htim = &htim3,
    .channel = TIM_CHANNEL_4,
    .TIM_start = HAL_TIM_Base_Start,
    .PWM_start = HAL_TIM_PWM_Start,
    .TIM_stop = HAL_TIM_Base_Stop,
    .PWM_stop = HAL_TIM_PWM_Stop
  };

  Motor_Handle motor = {
    .cw_handle = &cw_pwmHandler,
    .ccw_handle = &ccw_pwmHandler,
    .state = STOP,
    .rotary_handle = &rotary_handle,
    .pidIntegral = 0,
    .prevError = 0
  };

  int currentTicks = osKernelGetTickCount();
  hw_dcMotor_driveCW(&motor, 50);
  /* Infinite loop */
  for(;;)
  {
    // char msg[15];
    // sprintf(msg, "%d\t %d\r\n", io_adc_readPin(&adcHandler, PA0), hw_dcMotor_getSpeedCW(&motor));
    // HAL_UART_Transmit(&huart3,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


    // hw_dcMotor_setThrottleCW(&motor, 50);
    hw_dcMotor_setThrottleCW_PID(&motor, 40);
    hw_rotaryEncoder_resetCountCW(&rotary_handle);

    currentTicks += PERIOD;
    osDelayUntil(currentTicks);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

