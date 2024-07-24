#ifndef APP_DCMOTOR_H
#define APP_DCMOTOR_H
#endif //APP_DCMOTOR_H

#include <stdbool.h>
#include "io_pwm.h"

typedef struct {
    PWM_Handle* cw_handle;
    PWM_Handle* ccw_handle;
} DC_Motor_Handler;

void app_dcMotor_init(DC_Motor_Handler* handler);

void app_dcMotor_drive(DC_Motor_Handler* handler, uint8_t throttle, bool clockwise);