#include "hw_dcMotor.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define INTEGRAL_MIN -20
#define INTEGRAL_MAX 20

#define CLAMP(x) ((x) < (INTEGRAL_MIN) ? (INTEGRAL_MIN) : ((x) > (INTEGRAL_MAX) ? (INTEGRAL_MAX) : (x)))

#define MIN_MOTOR_THROTTLE 75.0
#define MAX_MOTOR_THROTTLE 100.0
#define THROTTLE_SCALE_FACTOR (MAX_MOTOR_THROTTLE - MIN_MOTOR_THROTTLE) / 100
#define MAP_THROTTLE(x) (MIN_MOTOR_THROTTLE + THROTTLE_SCALE_FACTOR * (float) (x))

#define MIN_RAW_SPEED 0
#define MAX_RAW_SPEED 20
#define MAP_SPEED(x) (((x) - (MIN_RAW_SPEED)) * ((MAX_MOTOR_THROTTLE) - (MIN_MOTOR_THROTTLE)) / ((MAX_RAW_SPEED) - (MIN_RAW_SPEED)) + (MIN_MOTOR_THROTTLE))


// uint8_t hw_dcMotor_throttleConvert(uint8_t throttle);
uint8_t hw_dcMotor_throttlePID(Motor_Handle *handle, uint8_t curr, uint8_t throttle);
void hw_dcMotor_setThrottle(Motor_Handle *handle, uint8_t throttle);

void hw_dcMotor_driveCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CW) return;

    if (handle->state == CCW) {
        hw_dcMotor_stop(handle);
    }
    io_pwm_start(handle->cw_handle,MAP_THROTTLE(throttle));
    handle->state = CW;
}

void hw_dcMotor_driveCCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CCW) return;

    if (handle->state == CW) {
        hw_dcMotor_stop(handle);
    }
    io_pwm_start(handle->ccw_handle, MAP_THROTTLE(throttle));
    handle->state = CCW;
}

void hw_dcMotor_setThrottle(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CW) {
        io_pwm_setDutyCycle(handle->cw_handle, throttle); // not mapping throttle
    } else if (handle->state == CCW) {
        io_pwm_setDutyCycle(handle->ccw_handle, throttle); // not mapping throttle
    }
}

void hw_dcMotor_tickThrottlePID(Motor_Handle *handle, uint8_t throttle) {
    hw_dcMotor_setThrottle(handle, hw_dcMotor_throttlePID(handle, hw_dcMotor_getSpeed(handle), throttle));
}

void hw_dcMotor_stop(Motor_Handle *handle) {
    if (handle->state == CW) {
        io_pwm_stop(handle->cw_handle);
    } else if (handle->state == CCW) {
        io_pwm_stop(handle->ccw_handle);
    }
    handle->state = MOTOR_STOP;
}

uint8_t hw_dcMotor_getSpeed(Motor_Handle *handle) {
    uint8_t speed = 0;

    if (handle->state == CW) {
        speed = MAP_SPEED(handle->rotary_handle->countCW);
        hw_rotaryEncoder_resetCountCW(handle->rotary_handle);
    }

    if (handle->state == CCW) {
        speed = MAP_SPEED(handle->rotary_handle->countCCW);
        hw_rotaryEncoder_resetCountCCW(handle->rotary_handle);
    }

    return speed;
}

uint8_t hw_dcMotor_throttlePID(Motor_Handle *handle, uint8_t curr, uint8_t throttle) {
    throttle = MAP_THROTTLE(throttle);

    int error = throttle - curr;
    uint8_t pidThrottle = throttle;

    // pidThrottle += handle->kP * error;
    //
    // handle->pidIntegral += handle->kI * error;
    // handle->pidIntegral = CLAMP(handle->pidIntegral);
    // pidThrottle += handle->pidIntegral;

    // pidThrottle += DERIVATIVE_COEFFICIENT * (error - handle->prevError);
    // handle->prevError = error;

    // char msg[47];
    // sprintf(msg, "throt: %d\t pid: %d\t curr: %d\t error: %d\r\n", throttle, pidThrottle, curr, error);
    // HAL_UART_Transmit(&huart3,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    return pidThrottle;
}