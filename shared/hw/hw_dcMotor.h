#ifndef APP_DCMOTOR_H
#define APP_DCMOTOR_H
#include <hw_rotaryEncoder.h>
#include <stdbool.h>
#endif //APP_DCMOTOR_H

#include "io_pwm.h"

typedef enum Motor_State {
    CW,
    CCW,
    MOTOR_STOP
} Motor_State;

typedef struct __Motor_Handle{
    PWM_Handle* cw_handle;
    PWM_Handle* ccw_handle;
    Motor_State state;
    Rotary_Handle* rotary_handle;
    uint8_t pidIntegral;
    uint8_t prevError;
    float kP;
    float kI;
} Motor_Handle;

/**
 * @param throttle between 0 and 100
 */
void hw_dcMotor_driveCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_driveCCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_tickThrottlePID(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_stop(Motor_Handle* handle);

/**
 * @return Speed, represented as a percentage between minimum and maximum speeds
 */
uint16_t hw_dcMotor_getSpeed(Motor_Handle* handle);