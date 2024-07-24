#ifndef IO_PWM_H
#define IO_PWM_H
#endif //IO_PWM_H

#include <tim.h>

typedef struct {
    TIM_HandleTypeDef* htim;

    HAL_StatusTypeDef (*TIM_start)(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef (*PWM_start)(TIM_HandleTypeDef *htim, uint32_t Channel);
} PWM_Handle;

void io_pwm_start(PWM_Handle* handler, uint32_t channel);

void io_pwm_setDutyCycle(PWM_Handle* handler, uint32_t channel, uint16_t dutyCycle);