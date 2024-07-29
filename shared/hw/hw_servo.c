//
// Created by jtssi on 2024-07-25.
//

#include "hw_servo.h"

#define PWM_FREQ 50
#define MIN_DCYCLE
#define MAX_DCYCLE
#define SERVO_CONVERT 100/180.0

void hw_servo_init(Servo_Handle *handle, uint8_t angle) {
    if (handle->state == OFF) {
        io_pwm_start(handle->angle_handle,hw_servo_angleToDuty(handle, angle));
        handle->state = ON;
    }
}

void hw_servo_end(Servo_Handle *handle) {
    io_pwm_stop(handle->angle_handle);
    handle->state = OFF;
}

void hw_servo_setAngle(Servo_Handle *handle, uint8_t angle) {
    if(handle->state==OFF)
        hw_servo_init(handle, angle);
    else
        io_pwm_setDutyCycle(handle->angle_handle, hw_servo_angleToDuty(handle, angle));
}

uint8_t hw_servo_angleToDuty(Servo_Handle *handle, uint8_t angle) {
    handle->currAngle = angle;
    return angle * SERVO_CONVERT;
}
