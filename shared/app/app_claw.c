#include "app_claw.h"

#define MIN_ANGLE 0 // TODO determine this
#define MAX_ANGLE 60 // TODO determine this
#define CLAMP_ANGLE(x) ((x) < MIN_ANGLE ? MIN_ANGLE : ((x) > MAX_ANGLE ? MAX_ANGLE : (x)))

#define ROUGH_ERROR_BOUND 50
#define PRECISE_ERROR_BOUND 10
#define WITHIN_ERROR(curr, goal, error) ((goal + error) >= curr && (goal - error) <= curr)

#define DEFAULT_LIFT_THROTTLE 100U
#define LIFT_BRAKE_THROTTLE 100U

void app_lift_initExtend(CLAW_Handle *handle, uint8_t throttle) {
    if (handle->__state == CLAW_UP) return;

    if (handle->__state == CLAW_DOWN) {
        app_claw_stopExtension(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = CLAW_UP;
}

void app_lift_initRetract(CLAW_Handle *handle, uint8_t throttle) {
    if (handle->__state == CLAW_DOWN) return;

    if (handle->__state == CLAW_UP) {
        app_claw_stopExtension(handle);
    }

    hw_dcMotor_drive(handle->dcMotor, -throttle); // TODO MIGHT BE BACKWARDS
    handle->__state = CLAW_DOWN;
}

void app_claw_initMoveToPos(CLAW_Handle *handle, uint16_t length) {
}

void app_claw_initMoveByLength(CLAW_Handle *handle, uint16_t length) {
}

void app_claw_tickMovePos(CLAW_Handle *handle) {
}

void app_claw_incrementPos(CLAW_Handle *handle) {
}

void app_claw_deIncrementPos(CLAW_Handle *handle) {
}

void app_claw_stopExtension(CLAW_Handle *handle) {
}

void app_claw_setAngle(CLAW_Handle *handle, uint8_t angle) {
}

void app_claw_initHome(CLAW_Handle *handle) {
}

void app_claw_tickHome(CLAW_Handle *handle) {
}

void app_claw_initCalibrateExtension(CLAW_Handle *handle) {
}

void app_claw_tickCalibrateExtension(CLAW_Handle *handle, UART_HandleTypeDef *uart) {
}
