#ifndef APP_DCMOTOR_H
#define APP_DCMOTOR_H
#include <stdbool.h>
#endif //APP_DCMOTOR_H

#include "io_pwm.h"

typedef struct __Motor_Handle{
 PWM_Handle* cw_handle;
 PWM_Handle* ccw_handle;
 bool drivingCW;
} Motor_Handle;

/**
 * @param throttle between 0 and 100
 */
void hw_dcMotor_driveCW(Motor_Handle* handle, uint8_t throttle);

/**
 * @param throttle between 0 and 100
 */
void hw_dcMotor_driveCCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_setThrottleCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_setThrottleCCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_stopCW(Motor_Handle* handle);

void hw_dcMotor_stopCCW(Motor_Handle* handle);