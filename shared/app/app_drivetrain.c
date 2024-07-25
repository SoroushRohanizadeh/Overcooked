#include "app_drivetrain.h"

void app_drivetrain_setDriveState(DT_Handle* handle, Drive_State state);

void app_drivetrain_driveUp(DT_Handle *handle, uint8_t throttle) {
    app_drivetrain_setDriveState(handle, UP);
    hw_dcMotor_driveCCW(handle->wheel_1, throttle);
    hw_dcMotor_driveCCW(handle->wheel_2, throttle);
    hw_dcMotor_driveCW(handle->wheel_3, throttle);
    hw_dcMotor_driveCW(handle->wheel_4, throttle);
}

void app_drivetrain_driveDown(DT_Handle *handle, uint8_t throttle) {
    app_drivetrain_setDriveState(handle, DOWN);
    hw_dcMotor_driveCW(handle->wheel_1, throttle);
    hw_dcMotor_driveCW(handle->wheel_2, throttle);
    hw_dcMotor_driveCCW(handle->wheel_3, throttle);
    hw_dcMotor_driveCCW(handle->wheel_4, throttle);
}

void app_drivetrain_driveLeft(DT_Handle *handle, uint8_t throttle) {
    app_drivetrain_setDriveState(handle, LEFT);
    hw_dcMotor_driveCW(handle->wheel_1, throttle);
    hw_dcMotor_driveCCW(handle->wheel_2, throttle);
    hw_dcMotor_driveCW(handle->wheel_3, throttle);
    hw_dcMotor_driveCCW(handle->wheel_4, throttle);
}

void app_drivetrain_driveRight(DT_Handle *handle, uint8_t throttle) {
    app_drivetrain_setDriveState(handle, RIGHT);
    hw_dcMotor_driveCCW(handle->wheel_1, throttle);
    hw_dcMotor_driveCW(handle->wheel_2, throttle);
    hw_dcMotor_driveCCW(handle->wheel_3, throttle);
    hw_dcMotor_driveCW(handle->wheel_4, throttle);
}

void app_drivetrain_tickThrottle(DT_Handle *handle, uint8_t throttle) {
    if (handle->state == UP) {
        hw_dcMotor_tickThrottlePID(handle->wheel_1, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_2, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_3, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_4, throttle);
    } else if (handle->state == DOWN) {
        hw_dcMotor_tickThrottlePID(handle->wheel_1, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_2, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_3, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_4, throttle);
    } else if (handle->state == LEFT) {
        hw_dcMotor_tickThrottlePID(handle->wheel_1, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_2, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_3, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_4, throttle);
    } else if (handle->state == RIGHT) {
        hw_dcMotor_tickThrottlePID(handle->wheel_1, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_2, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_3, throttle);
        hw_dcMotor_tickThrottlePID(handle->wheel_4, throttle);
    }
}

void app_drivetrain_stop(DT_Handle *handle) {
    hw_dcMotor_stop(handle->wheel_1);
    hw_dcMotor_stop(handle->wheel_2);
    hw_dcMotor_stop(handle->wheel_3);
    hw_dcMotor_stop(handle->wheel_4);
}

void app_drivetrain_setDriveState(DT_Handle* handle, Drive_State state) {
    app_drivetrain_stop(handle);
    handle->state = state;
}