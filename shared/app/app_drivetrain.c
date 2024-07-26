#include "app_drivetrain.h"

void app_drivetrain_setDriveState(DT_Handle* handle, Drive_State state);

void app_drivetrain_drive(DT_Handle *handle, uint8_t throttle[4], Drive_State state) {
    app_drivetrain_setDriveState(handle, state);

    if (handle->state == UP) {
        hw_dcMotor_driveCCW(handle->wheel_1, throttle[0]);
        hw_dcMotor_driveCCW(handle->wheel_2, throttle[1]);
        hw_dcMotor_driveCW(handle->wheel_3, throttle[2]);
        hw_dcMotor_driveCW(handle->wheel_4, throttle[3]);
    } else if (handle->state == DOWN) {
        hw_dcMotor_driveCW(handle->wheel_1, throttle[0]);
        hw_dcMotor_driveCW(handle->wheel_2, throttle[1]);
        hw_dcMotor_driveCCW(handle->wheel_3, throttle[2]);
        hw_dcMotor_driveCCW(handle->wheel_4, throttle[3]);
    } else if (handle->state == LEFT) {
        hw_dcMotor_driveCCW(handle->wheel_1, throttle[0]);
        hw_dcMotor_driveCW(handle->wheel_2, throttle[1]);
        hw_dcMotor_driveCCW(handle->wheel_3, throttle[2]);
        hw_dcMotor_driveCW(handle->wheel_4, throttle[3]);
    } else if (handle->state == RIGHT) {
        hw_dcMotor_driveCW(handle->wheel_1, throttle[0]);
        hw_dcMotor_driveCCW(handle->wheel_2, throttle[1]);
        hw_dcMotor_driveCW(handle->wheel_3, throttle[2]);
        hw_dcMotor_driveCCW(handle->wheel_4, throttle[3]);
    }
}


void app_drivetrain_tickThrottle(DT_Handle *handle, uint8_t throttle[4]) {
    if (handle->state != DRIVE_STOP) {
        hw_dcMotor_tickThrottlePID(handle->wheel_1, throttle[0]);
        hw_dcMotor_tickThrottlePID(handle->wheel_2, throttle[1]);
        hw_dcMotor_tickThrottlePID(handle->wheel_3, throttle[2]);
        hw_dcMotor_tickThrottlePID(handle->wheel_4, throttle[3]);
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