#ifndef APP_CLAW_H
#define APP_CLAW_H
#include <hw_dcMotor.h>
#include <hw_servo.h>
#include <stdbool.h>

#endif //APP_CLAW_H

typedef enum __CLAW_State {
    CLAW_UP,
    CLAW_DOWN,
    CLAW_STOPPED_Z
} CLAW_State;

typedef struct __CLAW_Handle {
    Servo_Handle* servo;
    Motor_Handle* dcMotor;
    GPIO_TypeDef* extendedBumperDef;
    uint16_t extendedBumperPin;
    GPIO_TypeDef* rectractedBumperDef;
    uint16_t rectractedBumperPin;
    uint16_t encoderCWPin;
    uint16_t encoderCCWPin;

    uint32_t __currPos; // in encoder ticks
    uint16_t __goalPos; // in encoder ticks
    uint16_t __maxExtension;
    CLAW_State __state;
    bool __calibFullyExtended;
} CLAW_Handle;

void app_claw_initMoveToPos(CLAW_Handle* handle, uint16_t length);
void app_claw_initMoveByLength(CLAW_Handle* handle, uint16_t length);
void app_claw_tickMovePos(CLAW_Handle* handle);

void app_claw_incrementPos(CLAW_Handle* handle);
void app_claw_deIncrementPos(CLAW_Handle* handle);

void app_claw_stopExtension(CLAW_Handle* handle);

void app_claw_setAngle(CLAW_Handle* handle, uint8_t angle);

// Homing
void app_claw_initHome(CLAW_Handle* handle);
void app_claw_tickHome(CLAW_Handle* handle);

// ONLY CALLED FOR CALIBRATION
void app_claw_initCalibrateExtension(CLAW_Handle* handle);
void app_claw_tickCalibrateExtension(CLAW_Handle* handle, UART_HandleTypeDef *uart);
