#include "hw_dcMotor.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define INTEGRAL_MIN -20
#define INTEGRAL_MAX 20

#define K_PROP 0.1
#define K_INT 0.01
#define K_DER 0.01

#define CLAMP(x) ((x) < (INTEGRAL_MIN) ? (INTEGRAL_MIN) : ((x) > (INTEGRAL_MAX) ? (INTEGRAL_MAX) : (x)))

#define MIN_MOTOR_DUTY_CYCLE 70.0
#define MAX_MOTOR_DUTY_CYCLE 100.0
#define DUTY_CYCLE_SCALE_FACTOR (MAX_MOTOR_DUTY_CYCLE - MIN_MOTOR_DUTY_CYCLE) / 100
#define MAP_DUTY_CYCLE(x) (MIN_MOTOR_DUTY_CYCLE + DUTY_CYCLE_SCALE_FACTOR * (float) (x))

void hw_dcMotor_setDutyCycle(Motor_Handle *handle, uint8_t dutyCycle);
uint8_t hw_dcMotor_speedToDutyCycle(Motor_Handle *handle, uint16_t newSpeed);

void hw_dcMotor_driveCW(Motor_Handle* handle, uint16_t speed) {
    if (handle->state == CW) return;

    if (handle->state == CCW) {
        hw_dcMotor_stop(handle);
    }

    handle->setSpeed = speed;
    io_pwm_start(handle->cw_handle, hw_dcMotor_speedToDutyCycle(handle, speed));
    handle->state = CW;
}

void hw_dcMotor_driveCCW(Motor_Handle* handle, uint16_t speed) {
    if (handle->state == CCW) return;

    if (handle->state == CW) {
        hw_dcMotor_stop(handle);
    }

    handle->setSpeed = speed;
    io_pwm_start(handle->ccw_handle, hw_dcMotor_speedToDutyCycle(handle, speed));
    handle->state = CCW;
}

void hw_dcMotor_setDutyCycle(Motor_Handle *handle, uint8_t dutyCycle) {
    if (handle->state == CW) {
        io_pwm_setDutyCycle(handle->cw_handle, dutyCycle);
    } else if (handle->state == CCW) {
        io_pwm_setDutyCycle(handle->ccw_handle, dutyCycle);
    }
}

void hw_dcMotor_tickSpeedPID(Motor_Handle *handle) {
    uint16_t curr = hw_dcMotor_getCurrentSpeed(handle);
    int error = handle->setSpeed - curr;
    uint16_t newSpeed = handle->setSpeed;
    // uint16_t newSpeed = handle->setSpeed + K_PROP * error;
    //
    // handle->pidIntegral += K_INT * error;
    // // handle->pidIntegral = CLAMP(handle->pidIntegral);
    // newSpeed += handle->pidIntegral;
    //
    // newSpeed += K_DER * (error - handle->prevError);
    // handle->prevError = error;

    // char msg[47];
    // sprintf(msg, "setSpeed: %d\t curr: %d\t error: %d\r\n", handle->setSpeed, curr, error);
    // HAL_UART_Transmit(&huart3,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    // handle->setSpeed = newSpeed;
    hw_dcMotor_setDutyCycle(handle, hw_dcMotor_speedToDutyCycle(handle, newSpeed));
}

uint8_t hw_dcMotor_speedToDutyCycle(Motor_Handle *handle, uint16_t newSpeed) {
    return (uint8_t) MAP_DUTY_CYCLE((100 * newSpeed) / handle->maxSpeed);
}

void hw_dcMotor_stop(Motor_Handle *handle) {
    if (handle->state == CW) {
        io_pwm_stop(handle->cw_handle);
    } else if (handle->state == CCW) {
        io_pwm_stop(handle->ccw_handle);
    }
    handle->state = MOTOR_STOP;
}

uint16_t hw_dcMotor_getCurrentSpeed(Motor_Handle *handle) {
    uint16_t speed = 0;

    if (handle->state == CW) {
        speed = handle->rotary_handle->countCW;
        hw_rotaryEncoder_resetCountCW(handle->rotary_handle);
    }

    if (handle->state == CCW) {
        speed = handle->rotary_handle->countCCW;
        hw_rotaryEncoder_resetCountCCW(handle->rotary_handle);
    }

    return speed;
}