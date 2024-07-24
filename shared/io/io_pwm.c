#include "io_pwm.h"

void io_pwm_start(PWM_Handle *handler) {
    handler->TIM_start(handler->htim);
    handler->PWM_start(handler->htim, handler->channel);
}

void io_pwm_setDutyCycle(PWM_Handle *handler, uint16_t dutyCycle) {
    __HAL_TIM_SetCompare(handler->htim, handler->channel, dutyCycle * handler->htim->Init.Period / 100);
}

void io_pwm_stop(PWM_Handle *handler) {
    handler->PWM_stop(handler->htim, handler->channel);
    handler->TIM_stop(handler->htim);
}
