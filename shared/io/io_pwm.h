#ifndef IO_PWM_H
#define IO_PWM_H
#endif //IO_PWM_H

#include <tim.h>

typedef struct {
    TIM_HandleTypeDef* htim;
    uint32_t channel;

    HAL_StatusTypeDef (*TIM_start)(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef (*PWM_start)(TIM_HandleTypeDef *htim, uint32_t Channel);

    HAL_StatusTypeDef (*TIM_stop)(TIM_HandleTypeDef *htim);
    HAL_StatusTypeDef (*PWM_stop)(TIM_HandleTypeDef *htim, uint32_t Channel);
} PWM_Handle;

void io_pwm_start(PWM_Handle* handler);

/**
 * @param dutyCycle between 0 and 100
 */
void io_pwm_setDutyCycle(PWM_Handle* handler, uint16_t dutyCycle);

void io_pwm_stop(PWM_Handle* handler);