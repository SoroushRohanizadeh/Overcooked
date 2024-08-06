#ifndef APP_LIFT_H
#define APP_LIFT_H
#include <hw_dcMotor.h>
#include <hw_servo.h>
#include <stdbool.h>
#endif //APP_LIFT_H

typedef enum __LIFT_State {
    LIFT_UP,
    LIFT_DOWN,
    LIFT_STOPPED_Z,
    LIFT_ARRIVED
} LIFT_State;

typedef struct __LIFT_Handle {
    Servo_Handle* servo;
    Motor_Handle* dcMotor;
    GPIO_TypeDef* topBumperDef;
    uint16_t topBumperPin;
    GPIO_TypeDef* bottomBumperDef;
    uint16_t bottomBumperPin;
    uint16_t encoderCWPin;
    uint16_t encoderCCWPin;

    uint32_t __currHeight; // in encoder ticks
    uint16_t __goalHeight; // in encoder ticks
    LIFT_State __state;
    bool __calibTopReached;

    void (*__tickFunction)(struct __LIFT_Handle *handle);
} LIFT_Handle;

void app_lift_initMoveToHeight(LIFT_Handle* handle, uint16_t height);
void app_lift_initMoveByHeight(LIFT_Handle* handle, int height);
void app_lift_tickMoveHeight(LIFT_Handle* handle);

void app_lift_initAlignHeight(LIFT_Handle* handle);
void app_lift_tickAlignHeight(LIFT_Handle* handle);

void app_lift_incrementHeight(LIFT_Handle* handle);
void app_lift_deIncrementHeight(LIFT_Handle* handle);

void app_lift_stopZ(LIFT_Handle* handle);

void app_lift_setX(LIFT_Handle* handle, uint8_t x);
void app_lift_setXForStack(LIFT_Handle* handle);

// Homing
void app_lift_initHome(LIFT_Handle* handle);
void app_lift_tickHome(LIFT_Handle* handle);

// ONLY CALLED FOR CALIBRATION
void app_lift_initCalibrateZ(LIFT_Handle* handle);
void app_lift_tickCalibrateZ(LIFT_Handle* handle, UART_HandleTypeDef *uart);