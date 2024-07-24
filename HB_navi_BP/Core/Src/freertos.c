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
#include <io_adc.h>
#include <app_analog.h>
#include <io_pwm.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <tim.h>
#include <usart.h>

#include "io_adc.h"
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
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
  UNUSED(hadc);
  adcConvCMPLT = true;
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
  int currentTicks = osKernelGetTickCount();

  volatile uint16_t adc_dma[NUM_PINS];

  enum ADC_Pin pins[] = {PA0, PA1};

  ADC_Handler adcHandler = {
    .hadcs = &hadc1,
    .twoADC = false,
    .numPins = NUM_PINS,
    .adcBuffer = adc_dma,
    .adcConvCMPLT = &adcConvCMPLT,
    .ADC_Start = &HAL_ADC_Start_DMA
  };

  PWM_Handle pwmHandler = {
    .htim = &htim1,
    .numPins = 1,
    .TIM_start = &HAL_TIM_Base_Start,
    .PWM_start = &HAL_TIM_PWM_Start
  };

  io_pwm_start(&pwmHandler, TIM_CHANNEL_1);
  // int dutyCycle = 0;

  app_analog_init(&adcHandler, pins);
  /* Infinite loop */
  for(;;)
  {
    char msg[15];
    sprintf(msg, "%d\t %d\r\n", app_analog_readPin(PA0), app_analog_readPin(PA1));
    HAL_UART_Transmit(&huart1,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    // __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, app_analog_readPin(PA0) * 1600 / 4055);
    io_pwm_setDutyCycle(&pwmHandler, TIM_CHANNEL_1, app_analog_readPin(PA0) * 1600 / 4055);
    // dutyCycle = (dutyCycle == 1600 ? 0 : dutyCycle + 100);

    currentTicks += PERIOD;
    osDelayUntil(currentTicks);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

