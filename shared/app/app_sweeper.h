#ifndef APP_SWEEPER_H
#define APP_SWEEPER_H
#include <hw_dcMotor.h>
#include <stdbool.h>

#endif //APP_SWEEPER_H

typedef enum __SWEEP_State {
    SWEEP_LEFT,
    SWEEP_RIGHT,
    SWEEP_STOP
} SWEEP_State;

typedef struct __SWEEP_Handle {
    Motor_Handle* dcMotor;
    GPIO_TypeDef* leftBumperDef;
    uint16_t leftBumperPin;
    GPIO_TypeDef* rightBumperDef;
    uint16_t rightBumperPin;
    uint16_t encoderCWPin;
    uint16_t encoderCCWPin;

    uint32_t __currPos; // in encoder ticks with respect to the left limit
    uint16_t __goalPos; // in encoder ticks with respect to the left limit
    SWEEP_State __state;
    bool __calibFullyLeft;
} SWEEP_Handle;

void app_sweeper_initMoveToPos(SWEEP_Handle* handle, uint16_t pos);
void app_sweeper_initMoveByLength(SWEEP_Handle* handle, uint16_t length);
void app_sweeper_tickMovePos(SWEEP_Handle* handle);

void app_sweeper_initAlignPos(SWEEP_Handle* handle);
void app_sweeper_tickAlignPos(SWEEP_Handle* handle);

void app_sweeper_incrementPos(SWEEP_Handle* handle);
void app_sweeper_deIncrementPos(SWEEP_Handle* handle);

void app_sweeper_stop(SWEEP_Handle* handle);

// Homing
void app_sweeper_initHome(SWEEP_Handle* handle);
void app_sweeper_tickHome(SWEEP_Handle* handle);

// ONLY CALLED FOR CALIBRATION
void app_sweeper_initCalibrateExtension(SWEEP_Handle* handle);
void app_sweeper_tickCalibrateExtension(SWEEP_Handle* handle, UART_HandleTypeDef *uart);