#ifndef APP_CLAW_H
#define APP_CLAW_H
#include <hw_dcMotor.h>
#include <hw_servo.h>
#include <stdbool.h>

#endif //APP_CLAW_H

typedef enum __CLAW_State {
    CLAW_EXTENDING,
    CLAW_CONTRACTING,
    CLAW_STOP_EXTEND
} CLAW_State;

typedef struct __CLAW_Handle {
    Servo_Handle* servo;
    Motor_Handle* dcMotor;
    GPIO_TypeDef* topBumperDef;
    uint16_t topBumperPin;
    GPIO_TypeDef* bottomBumperDef;
    uint16_t bottomBumperPin;

    uint8_t __height;
    CLAW_State __state;
    bool __calibTopReached;
} CLAW_Handle;

void app_claw_extend(CLAW_Handle* handle);

void app_claw_contract(CLAW_Handle* handle);

void app_claw_stop(CLAW_Handle* handle);

void app_claw_setX(CLAW_Handle* handle, uint8_t x);


// ONLY CALLED FOR CALIBRATION
void app_claw_initCalibrateZ(CLAW_Handle* handle);

void app_claw_tickCalibrateZ(CLAW_Handle* handle, UART_HandleTypeDef *uart);