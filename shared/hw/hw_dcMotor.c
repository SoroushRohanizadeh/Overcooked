#include "hw_dcMotor.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define PROPORTIONAL_COEFFICIENT 1.0
#define INTEGRAL_COEFFICIENT 1.0

#define INTEGRAL_MIN -2
#define INTEGRAL_MAX 2

#define CLAMP(x) ((x) < (INTEGRAL_MIN) ? (INTEGRAL_MIN) : ((x) > (INTEGRAL_MAX) ? (INTEGRAL_MAX) : (x)))

#define MIN_MOTOR_THROTTLE 75.0
#define MAX_MOTOR_THROTTLE 100.0
#define THROTTLE_SCALE_FACTOR (MAX_MOTOR_THROTTLE - MIN_MOTOR_THROTTLE) / 100
#define MAP_THROTTLE(x) (MIN_MOTOR_THROTTLE + THROTTLE_SCALE_FACTOR * (float) (x))

#define MIN_RAW_SPEED 0
#define MAX_RAW_SPEED 20
#define MAP_SPEED(x) (((x) - (MIN_RAW_SPEED)) * ((MAX_MOTOR_THROTTLE) - (MIN_MOTOR_THROTTLE)) / ((MAX_RAW_SPEED) - (MIN_RAW_SPEED)) + (MIN_MOTOR_THROTTLE))


// uint8_t hw_dcMotor_throttleConvert(uint8_t throttle);
uint8_t hw_dcMotor_throttlePID(uint8_t curr, uint8_t throttle);

void hw_dcMotor_driveCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CW) return;

    if (handle->state == CCW) {
        hw_dcMotor_stopCCW(handle);
    }
    io_pwm_start(handle->cw_handle,MAP_THROTTLE(throttle));
}

void hw_dcMotor_driveCCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CCW) return;

    if (handle->state == CW) {
        hw_dcMotor_stopCCW(handle);
    }
    io_pwm_start(handle->ccw_handle, MAP_THROTTLE(throttle));
}

void hw_dcMotor_setThrottleCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->cw_handle, throttle); // not mapping throttle
}

void hw_dcMotor_setThrottleCCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->ccw_handle, MAP_THROTTLE(throttle));
}

void hw_dcMotor_setThrottleCW_PID(Motor_Handle *handle, uint8_t throttle) {
    hw_dcMotor_setThrottleCW(handle, hw_dcMotor_throttlePID(hw_dcMotor_getSpeedCW(handle), throttle));
}

void hw_dcMotor_setThrottleCCW_PID(Motor_Handle *handle, uint8_t throttle) {
    hw_dcMotor_setThrottleCCW(handle, hw_dcMotor_throttlePID(hw_dcMotor_getSpeedCW(handle), throttle));
}

void hw_dcMotor_stopCW(Motor_Handle *handle) {
    io_pwm_stop(handle->cw_handle);
}

void hw_dcMotor_stopCCW(Motor_Handle *handle) {
    io_pwm_stop(handle->ccw_handle);
}

uint8_t hw_dcMotor_getSpeedCW(Motor_Handle *handle) {
    return MAP_SPEED(handle->rotary_handle->countCW);
}

uint8_t hw_dcMotor_getSpeedCCW(Motor_Handle *handle) {
    return MAP_SPEED(handle->rotary_handle->countCCW);
}

// uint8_t hw_dcMotor_throttleConvert(uint8_t throttle) {
//     return MIN_MOTOR_THROTTLE + THROTTLE_SCALE_FACTOR * (float) throttle;
// }

uint8_t hw_dcMotor_throttlePID(uint8_t curr, uint8_t throttle) {
    throttle = MAP_THROTTLE(throttle);

    int error = throttle - curr;
    uint8_t pidThrottle = throttle;

    pidThrottle += PROPORTIONAL_COEFFICIENT * error;
    // handle->pidIntegral += INTEGRAL_COEFFICIENT * error;
    // handle->pidIntegral = CLAMP(handle->pidIntegral);
    // pidThrottle += handle->pidIntegral;

    char msg[34];
    sprintf(msg, "throt: %d\t pid: %d\t curr: %d\r\n", throttle, pidThrottle, curr);
    HAL_UART_Transmit(&huart3,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    return pidThrottle;
}