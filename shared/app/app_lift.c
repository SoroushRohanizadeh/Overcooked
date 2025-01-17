#include "app_lift.h"

#include <stdio.h>
#include <string.h>

#define DEFAULT_LIFT_X 20U // TODO determine this

#define MIN_X_LOCATION 0U // TODO determine this using calib functions
#define MAX_X_LOCATION 180U // TODO determine this using calib functions
#define CLAMP_ANGLE(x) ((x) < MIN_X_LOCATION ? MIN_X_LOCATION : ((x) > MAX_X_LOCATION ? MAX_X_LOCATION : (x)))

#define ROUGH_ERROR_BOUND 50 // in encoder ticks middle finger
#define PRECISE_ERROR_BOUND 10
#define WITHIN_ERROR(curr, goal, error) ((goal + error) >= curr && (goal - error) <= curr)

#define DEFAULT_LIFT_THROTTLE 100U
#define BRAKE_THROTTLE 100U
#define ALIGNMENT_THROTTLE 20U
#define HOMING_THROTTLE 20U

void app_lift_initMoveUp(LIFT_Handle *handle, uint8_t throttle) {
    if (handle->__state == LIFT_UP) return;

    if (handle->__state == LIFT_DOWN) {
        app_lift_stopZ(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = LIFT_UP;
}

void app_lift_initMoveDown(LIFT_Handle *handle, uint8_t throttle) {
    if (handle->__state == LIFT_DOWN) return;

    if (handle->__state == LIFT_UP) {
        app_lift_stopZ(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, -throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = LIFT_DOWN;
}

void app_lift_tickHeight(LIFT_Handle* handle) {
    if (WITHIN_ERROR(handle->__currHeight, handle->__goalHeight, ROUGH_ERROR_BOUND)) {
        app_lift_stopZ(handle);
        app_lift_initAlignHeight(handle);
        handle->__tickFunction = app_lift_tickAlignHeight;
    }
}

void app_lift_initMoveToHeight(LIFT_Handle *handle, uint16_t height) {
    handle->__goalHeight = height;

    if (height > handle->__currHeight) {
        app_lift_initMoveUp(handle, DEFAULT_LIFT_THROTTLE);
    } else {
        app_lift_initMoveDown(handle, DEFAULT_LIFT_THROTTLE);
    }

    handle->__tickFunction = app_lift_tickHeight;
}

void app_lift_tickMoveHeight(LIFT_Handle *handle) {
    handle->__tickFunction(handle);
}

void app_lift_initAlignHeight(LIFT_Handle *handle) {
    if (handle->__currHeight < handle->__goalHeight) {
        app_lift_initMoveUp(handle, ALIGNMENT_THROTTLE);
    } else if (handle->__currHeight > handle->__goalHeight) {
        app_lift_initMoveDown(handle, ALIGNMENT_THROTTLE);
    }
}

void app_lift_tickArrived(LIFT_Handle *handle) {
    UNUSED(handle);
    // do nothing
}

void app_lift_tickAlignHeight(LIFT_Handle *handle) {
    if (WITHIN_ERROR(handle->__currHeight, handle->__goalHeight, PRECISE_ERROR_BOUND)) {
        hw_dcMotor_stop(handle->dcMotor);
        handle->__state == LIFT_ARRIVED;
        handle->__tickFunction = app_lift_tickArrived;
    }
}

void app_lift_incrementHeight(LIFT_Handle *handle) {
    handle->__currHeight++;
}

void app_lift_deIncrementHeight(LIFT_Handle *handle) {
    handle->__currHeight--;
}

void app_lift_initMoveByHeight(LIFT_Handle *handle, int height) {
    app_lift_initMoveToHeight(handle, handle->__currHeight + height);
}

void app_lift_stopZ(LIFT_Handle *handle) {
    if (handle->__state == LIFT_STOPPED_Z) return;

    LIFT_State state = handle->__state;
    hw_dcMotor_stop(handle->dcMotor);

    if (state == LIFT_UP) {
        app_lift_initMoveDown(handle, BRAKE_THROTTLE);
    } else {
        app_lift_initMoveUp(handle, BRAKE_THROTTLE);
    }

    hw_dcMotor_stop(handle->dcMotor);
    handle->__state = LIFT_STOPPED_Z;
}

void app_lift_setX(LIFT_Handle* handle, uint8_t x) {
    hw_servo_setAngle(handle->servo, CLAMP_ANGLE(x));
}

void app_lift_setXForStack(LIFT_Handle *handle) {
    app_lift_setX(handle, DEFAULT_LIFT_X);
}

void app_lift_initHome(LIFT_Handle *handle) {
    app_lift_initMoveDown(handle, HOMING_THROTTLE);
}

void app_lift_tickHome(LIFT_Handle *handle) {
    if (!HAL_GPIO_ReadPin(handle->bottomBumperDef, handle->bottomBumperPin)) {
        app_lift_stopZ(handle);
        handle->__currHeight = 0;
    }
}

void app_lift_initCalibrateZ(LIFT_Handle *handle) {
    app_lift_initMoveUp(handle, DEFAULT_LIFT_THROTTLE);
    handle->__calibTopReached = false;
}

void app_lift_tickCalibrateZ(LIFT_Handle *handle, UART_HandleTypeDef *uart) {
    if (!handle->__calibTopReached) {
        if (!HAL_GPIO_ReadPin(handle->topBumperDef, handle->topBumperPin)) {
            app_lift_stopZ(handle);
            handle->__currHeight = 0;
            app_lift_initMoveDown(handle, DEFAULT_LIFT_THROTTLE);
        }
    } else {
        if (!HAL_GPIO_ReadPin(handle->bottomBumperDef, handle->bottomBumperPin)) {
            app_lift_stopZ(handle);
            char msg[21];
            sprintf(msg, "Ticks: %ld\r\n", handle->__currHeight);
            HAL_UART_Transmit(uart, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
        }
    }
}