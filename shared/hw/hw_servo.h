//
// Created by jtssi on 2024-07-25.
//

#ifndef HW_SERVO_H
#define HW_SERVO_H
#include <hw_dcMotor.h>

#endif //HW_SERVO_H

typedef enum Servo_State {
    ON,
    OFF
} Servo_State;

typedef struct __Servo_Handle{
    PWM_Handle* angle_handle;
    Servo_State state;
    uint8_t currAngle;
} Servo_Handle;


void hw_servo_init(Servo_Handle* handle, uint8_t angle);

void hw_servo_end(Servo_Handle* handle);

void hw_servo_setAngle(Servo_Handle* handle, uint8_t angle);

uint8_t hw_servo_angleToDuty(Servo_Handle *handle, uint8_t angle);
