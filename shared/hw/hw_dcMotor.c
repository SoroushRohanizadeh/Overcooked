#include "hw_dcMotor.h"

#define MIN_MOTOR_THROTTLE 75
#define MAX_MOTOR_THROTTLE 100

#define THROTTLE_SCALE_FACTOR (MAX_MOTOR_THROTTLE - MIN_MOTOR_THROTTLE) / 100

uint8_t hw_dcMotor_throttleConvert(uint8_t throttle);

void hw_dcMotor_driveCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CW) return;

    if (handle->state == CCW) {
        hw_dcMotor_stopCCW(handle);
    }
    io_pwm_start(handle->cw_handle,hw_dcMotor_throttleConvert(throttle));
}

void hw_dcMotor_driveCCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->state == CCW) return;

    if (handle->state == CW) {
        hw_dcMotor_stopCCW(handle);
    }
    io_pwm_start(handle->ccw_handle, hw_dcMotor_throttleConvert(throttle));
}

void hw_dcMotor_setThrottleCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->cw_handle, hw_dcMotor_throttleConvert(throttle));
}

void hw_dcMotor_setThrottleCCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->cw_handle, hw_dcMotor_throttleConvert(throttle));
}

void hw_dcMotor_stopCW(Motor_Handle *handle) {
    io_pwm_stop(handle->cw_handle);
}

void hw_dcMotor_stopCCW(Motor_Handle *handle) {
    io_pwm_stop(handle->ccw_handle);
}

uint16_t hw_dcMotor_speedCW(Motor_Handle *handle) {
    return handle->rotary_handle->countCW;
}

uint16_t hw_dcMotor_speedCCW(Motor_Handle *handle) {
    return handle->rotary_handle->countCCW;
}

uint8_t hw_dcMotor_throttleConvert(uint8_t throttle) {
    return MIN_MOTOR_THROTTLE + THROTTLE_SCALE_FACTOR * (float) throttle;
}