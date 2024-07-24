#include "hw_dcMotor.h"


void hw_dcMotor_driveCW(Motor_Handle *handle, uint8_t throttle) {
    if (!handle->drivingCW) hw_dcMotor_stopCCW(handle);
    io_pwm_start(handle->cw_handle, throttle);
}

void hw_dcMotor_driveCCW(Motor_Handle *handle, uint8_t throttle) {
    if (handle->drivingCW) hw_dcMotor_stopCW(handle);
    io_pwm_start(handle->ccw_handle, throttle);
}

void hw_dcMotor_setThrottleCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->cw_handle, throttle);
}

void hw_dcMotor_setThrottleCCW(Motor_Handle *handle, uint8_t throttle) {
    io_pwm_setDutyCycle(handle->cw_handle, throttle);
}

void hw_dcMotor_stopCW(Motor_Handle *handle) {
    io_pwm_stop(handle->cw_handle);
}

void hw_dcMotor_stopCCW(Motor_Handle *handle) {
    io_pwm_stop(handle->ccw_handle);
}
