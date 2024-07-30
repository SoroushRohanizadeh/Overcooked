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
    uint16_t maxSpeed; // in ticks per 10ms
    uint16_t setSpeed; // the speed set by the control algorithm
} Motor_Handle;

void hw_dcMotor_driveCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_driveCCW(Motor_Handle* handle, uint8_t throttle);

void hw_dcMotor_tickSpeedPID(Motor_Handle* handle);

void hw_dcMotor_stop(Motor_Handle* handle);

void hw_dcMotor_setThrottle(Motor_Handle *handle, uint8_t throttle);

uint16_t hw_dcMotor_getCurrentSpeed(Motor_Handle* handle);