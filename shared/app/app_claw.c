#include "app_claw.h"

#include <stdio.h>
#include <string.h>

#define MIN_ANGLE 0 // TODO determine this
#define MAX_ANGLE 60 // TODO determine this
#define CLAMP_ANGLE(x) ((x) < MIN_ANGLE ? MIN_ANGLE : ((x) > MAX_ANGLE ? MAX_ANGLE : (x)))

#define ROUGH_ERROR_BOUND 50
#define PRECISE_ERROR_BOUND 10
#define WITHIN_ERROR(curr, goal, error) ((goal + error) >= curr && (goal - error) <= curr)

#define DEFAULT_CLAW_THROTTLE 100U
#define CLAW_BRAKE_THROTTLE 100U
#define CALIB_THROTTLE 20U
#define HOMING_THROTTLE 20U


void app_claw_initExtend(CLAW_Handle *handle, uint8_t throttle) {
    if (handle->__state == CLAW_UP) return;

    if (handle->__state == CLAW_DOWN) {
        app_claw_stopExtension(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = CLAW_UP;
}

void app_claw_initRetract(CLAW_Handle *handle, uint8_t throttle) {
    if (handle->__state == CLAW_DOWN) return;

    if (handle->__state == CLAW_UP) {
        app_claw_stopExtension(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, -throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = CLAW_DOWN;
}

void app_claw_initMoveToPos(CLAW_Handle *handle, uint16_t pos) {
    handle->__goalPos = pos;

    if (pos > handle->__currPos) {
        app_claw_initRetract(handle, DEFAULT_CLAW_THROTTLE);
    } else {
        app_claw_initExtend(handle, DEFAULT_CLAW_THROTTLE);
    }
}

void app_claw_initMoveByLength(CLAW_Handle *handle, uint16_t length) {
    app_claw_initMoveToPos(handle, handle->__currPos + length);
}

void app_claw_tickMovePos(CLAW_Handle *handle) {
    if (WITHIN_ERROR(handle->__currPos, handle->__goalPos, ROUGH_ERROR_BOUND)) {
        app_claw_stopExtension(handle);
    }
}

void app_claw_incrementPos(CLAW_Handle *handle) {
    handle->__currPos++;
}

void app_claw_deIncrementPos(CLAW_Handle *handle) {
    handle->__currPos--;
}

void app_claw_stopExtension(CLAW_Handle *handle) {
    if (handle->__state == CLAW_STOPPED_EXT) return;

    CLAW_State state = handle->__state;
    hw_dcMotor_stop(handle->dcMotor);

    if (state == CLAW_STOPPED_EXT) {
        app_claw_initRetract(handle, CLAW_BRAKE_THROTTLE);
    } else {
        app_claw_initExtend(handle, CLAW_BRAKE_THROTTLE);
    }

    hw_dcMotor_stop(handle->dcMotor);
    handle->__state = CLAW_STOPPED_EXT;
}

void app_claw_setAngle(CLAW_Handle *handle, uint8_t angle) {
    hw_servo_setAngle(handle->servo, CLAMP_ANGLE(angle));
}

void app_claw_initHome(CLAW_Handle *handle) {
    app_claw_initRetract(handle, HOMING_THROTTLE);
}

void app_claw_tickHome(CLAW_Handle *handle) {
    if (!HAL_GPIO_ReadPin(handle->extendedBumperDef, handle->extendedBumperPin)) {
        app_claw_stopExtension(handle);
        handle->__currPos = 0;
    }
}

void app_claw_initCalibrateExtension(CLAW_Handle *handle) {
    app_claw_initRetract(handle, CALIB_THROTTLE);
    handle->__calibFullyExtended = false;
}

void app_claw_tickCalibrateExtension(CLAW_Handle *handle, UART_HandleTypeDef *uart) {
    if (!handle->__calibFullyExtended) {
        if (!HAL_GPIO_ReadPin(handle->extendedBumperDef, handle->extendedBumperPin)) {
            app_claw_stopExtension(handle);
            handle->__currPos = 0;
            app_claw_initRetract(handle, CALIB_THROTTLE);
        }
    } else {
        if (!HAL_GPIO_ReadPin(handle->rectractedBumperDef, handle->rectractedBumperPin)) {
            app_claw_stopExtension(handle);
            char msg[21];
            sprintf(msg, "Ticks: %ld\r\n", handle->__currPos);
            HAL_UART_Transmit(uart, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
        }
    }
}
