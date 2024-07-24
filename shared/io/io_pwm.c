//
// Created by rohan on 2024-07-23.
//

#include "io_pwm.h"

void io_pwm_start(PWM_Handle *handler, uint32_t channel) {
    handler->TIM_start(handler->htim);
    handler->PWM_start(handler->htim, channel);
}

void io_pwm_setDutyCycle(PWM_Handle *handler, uint32_t channel, uint16_t dutyCycle) {
    __HAL_TIM_SetCompare(handler->htim, TIM_CHANNEL_1, dutyCycle);
}
