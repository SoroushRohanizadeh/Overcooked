#include "hw_servo.h"

#define MIN_DCYCLE
#define MAX_DCYCLE
#define ANGLE_CONVERT(x) ((x) * 10 / 180 + 2)

float hw_servo_angleToDuty(Servo_Handle *handle, uint8_t angle);

void hw_servo_init(Servo_Handle *handle, uint8_t angle) {
    if (handle->state == OFF) {
        io_pwm_startFloat(handle->pwmHandle, hw_servo_angleToDuty(handle, angle));
        handle->state = ON;
    }
}

void hw_servo_end(Servo_Handle *handle) {
    io_pwm_stop(handle->pwmHandle);
    handle->state = OFF;
}

void hw_servo_setAngle(Servo_Handle *handle, uint8_t angle) {
    if(handle->state==OFF) {
        hw_servo_init(handle, angle);
    } else {
        io_pwm_setDutyCycleFloat(handle->pwmHandle, hw_servo_angleToDuty(handle, angle));
    }
}

float hw_servo_angleToDuty(Servo_Handle *handle, uint8_t angle) {
    handle->currAngle = angle;
    return ANGLE_CONVERT(angle);
}