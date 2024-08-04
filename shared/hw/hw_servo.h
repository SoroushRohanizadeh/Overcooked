#ifndef HW_SERVO_H
#define HW_SERVO_H
#endif //HW_SERVO_H

#include <io_pwm.h>

typedef enum __Servo_State {
    ON,
    OFF
} Servo_State;

typedef struct __Servo_Handle{
    PWM_Handle* pwmHandle;
    Servo_State state;
    uint8_t currAngle;
} Servo_Handle;

void hw_servo_init(Servo_Handle* handle, uint8_t angle);

void hw_servo_setAngle(Servo_Handle* handle, uint8_t angle);

void hw_servo_end(Servo_Handle* handle);