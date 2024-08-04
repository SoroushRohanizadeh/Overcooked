#ifndef APP_LIFT_H
#define APP_LIFT_H
#include <hw_dcMotor.h>
#include <hw_servo.h>
#include <stdbool.h>
#endif //APP_LIFT_H

typedef enum __LIFT_State {
    LIFT_UP,
    LIFT_DOWN,
    LIFT_STOPPED_Z
} LIFT_State;

typedef struct __LIFT_Handle {
    Servo_Handle* servo;
    Motor_Handle* dcMotor;
    GPIO_TypeDef* topBumperDef;
    uint16_t topBumperPin;
    GPIO_TypeDef* bottomBumperDef;
    uint16_t bottomBumperPin;

    uint8_t __height;
    LIFT_State __state;
    bool calibTopReached;
} LIFT_Handle;

void app_lift_moveUp(LIFT_Handle* handle);

void app_lift_moveDown(LIFT_Handle* handle);

void app_lift_stop(LIFT_Handle* handle);

void app_lift_setX(LIFT_Handle* handle, uint8_t x);


// ONLY CALLED FOR CALIBRATION
void app_lift_initCalibrateZ(LIFT_Handle* handle);

void app_lift_tickCalibrateZ(LIFT_Handle* handle, UART_HandleTypeDef *uart);