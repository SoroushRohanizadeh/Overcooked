#include "app_sweeper.h"

#include <stdio.h>
#include <string.h>

#define ROUGH_ERROR_BOUND 50U
#define PRECISE_ERROR_BOUND 10U
#define WITHIN_ERROR(curr, goal, error) ((goal + error) >= curr && (goal - error) <= curr)

#define DEFAULT_SWEEP_THROTTLE 100U
#define BRAKE_THROTTLE 100U
#define ALIGNMENT_THROTTLE 20U
#define CALIB_THROTTLE 20U
#define HOMING_THROTTLE 20U

void app_sweeper_initRight(SWEEP_Handle *handle, uint8_t throttle) {
    if (handle->__state == SWEEP_RIGHT) return;

    if (handle->__state == SWEEP_LEFT) {
        app_sweeper_stop(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = SWEEP_RIGHT;
}

void app_sweeper_initLeft(SWEEP_Handle *handle, uint8_t throttle) {
    if (handle->__state == SWEEP_LEFT) return;

    if (handle->__state == SWEEP_RIGHT) {
        app_sweeper_stop(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, -throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = SWEEP_LEFT;
}

void app_sweeper_initMoveToPos(SWEEP_Handle *handle, uint16_t pos) {
    handle->__goalPos = pos;

    if (pos > handle->__currPos) {
        app_sweeper_initLeft(handle, DEFAULT_SWEEP_THROTTLE);
    } else {
        app_sweeper_initRight(handle, DEFAULT_SWEEP_THROTTLE);
    }
}

void app_sweeper_initMoveByLength(SWEEP_Handle *handle, uint16_t length) {
    app_sweeper_initMoveToPos(handle, handle->__currPos + length);
}

void app_sweeper_tickMovePos(SWEEP_Handle *handle) {
    if (WITHIN_ERROR(handle->__currPos, handle->__goalPos, ROUGH_ERROR_BOUND)) {
        app_sweeper_stop(handle);
    }
}

void app_sweeper_initAlignPos(SWEEP_Handle *handle) {
    if (handle->__currPos < handle->__goalPos) {
        app_sweeper_initRight(handle, ALIGNMENT_THROTTLE);
    } else if (handle->__currPos > handle->__goalPos) {
        app_sweeper_initLeft(handle, ALIGNMENT_THROTTLE);
    }
}

void app_sweeper_tickAlignPos(SWEEP_Handle *handle) {
    if (WITHIN_ERROR(handle->__currPos, handle->__goalPos, PRECISE_ERROR_BOUND)) {
        hw_dcMotor_stop(handle->dcMotor);
    }
}

void app_sweeper_incrementPos(SWEEP_Handle *handle) {
    handle->__currPos++;
}

void app_sweeper_deIncrementPos(SWEEP_Handle *handle) {
    handle->__currPos--;
}

void app_sweeper_stop(SWEEP_Handle *handle) {
    if (handle->__state == SWEEP_STOP) return;

    SWEEP_State state = handle->__state;
    hw_dcMotor_stop(handle->dcMotor);

    if (state == SWEEP_LEFT) {
        app_sweeper_initRight(handle, BRAKE_THROTTLE);
    } else {
        app_sweeper_initLeft(handle, BRAKE_THROTTLE);
    }

    hw_dcMotor_stop(handle->dcMotor);
    handle->__state = SWEEP_STOP;
}

void app_sweeper_initHome(SWEEP_Handle *handle) {
    app_sweeper_initLeft(handle, HOMING_THROTTLE);
}

void app_sweeper_tickHome(SWEEP_Handle *handle) {
    if (!HAL_GPIO_ReadPin(handle->leftBumperDef, handle->leftBumperPin)) {
        app_sweeper_stop(handle);
        handle->__currPos = 0;
    }
}

void app_sweeper_initCalibrateExtension(SWEEP_Handle *handle) {
    app_sweeper_initLeft(handle, CALIB_THROTTLE);
    handle->__calibFullyLeft = false;
}

void app_sweeper_tickCalibrateExtension(SWEEP_Handle *handle, UART_HandleTypeDef *uart) {
    if (!handle->__calibFullyLeft) {
        if (!HAL_GPIO_ReadPin(handle->leftBumperDef, handle->leftBumperPin)) {
            app_sweeper_stop(handle);
            handle->__currPos = 0;
            app_sweeper_initRight(handle, CALIB_THROTTLE);
        }
    } else {
        if (!HAL_GPIO_ReadPin(handle->rightBumperDef, handle->rightBumperPin)) {
            app_sweeper_stop(handle);
            char msg[21];
            sprintf(msg, "Ticks: %ld\r\n", handle->__currPos);
            HAL_UART_Transmit(uart, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
        }
    }
}
