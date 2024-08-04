#include "app_lift.h"

#include <stdio.h>
#include <string.h>

#define MIN_X_LOCATION 0 // TODO determine this using calib functions
#define MAX_X_LOCATION 180 // TODO determine this using calib functions
#define CLAMP_ANGLE(x) ((x) < MIN_X_LOCATION ? MIN_X_LOCATION : ((x) > MAX_X_LOCATION ? MAX_X_LOCATION : (x)))

#define DEFAULT_LIFT_THROTTLE 100U

void app_lift_moveUp(LIFT_Handle *handle) {
    if (handle->__state == LIFT_UP) return;

    if (handle->__state == LIFT_DOWN) {
        app_lift_stop(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, DEFAULT_LIFT_THROTTLE); // TODO MIGHT BE BACKWARDS
    handle->__state = LIFT_UP;
}

void app_lift_moveDown(LIFT_Handle *handle) {
    if (handle->__state == LIFT_DOWN) return;

    if (handle->__state == LIFT_UP) {
        app_lift_stop(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, -DEFAULT_LIFT_THROTTLE); // TODO MIGHT BE BACKWARDS
    handle->__state = LIFT_DOWN;
}

void app_lift_stop(LIFT_Handle *handle) {
    if (handle->__state == LIFT_STOPPED_Z) return;

    hw_dcMotor_stop(handle->dcMotor);
    handle->__state = LIFT_STOPPED_Z;
}

void app_lift_setX(LIFT_Handle* handle, uint8_t x) {
    hw_servo_setAngle(handle->servo, CLAMP_ANGLE(x));
}

void app_lift_initCalibrateZ(LIFT_Handle *handle) {
    app_lift_moveUp(handle);
    handle->calibTopReached = false;
}

void app_lift_tickCalibrateZ(LIFT_Handle *handle, UART_HandleTypeDef *uart) {
    if (!handle->calibTopReached) {
        if (HAL_GPIO_ReadPin(handle->topBumperDef, handle->topBumperPin)) {
            app_lift_stop(handle);
            hw_rotaryEncoder_resetCount(handle->dcMotor->rotary_handle);
        }
    } else {
        if (HAL_GPIO_ReadPin(handle->bottomBumperDef, handle->bottomBumperPin)) {
            app_lift_stop(handle);
            char msg[10];
            sprintf(msg, "Ticks: %d\r\n", hw_rotaryEncoder_getCount(handle->dcMotor->rotary_handle));
            HAL_UART_Transmit(uart, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
        }
    }

}