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
    if (handle->state == DRIVE_STOP) return;

    hw_dcMotor_setThrottle(handle->wheel_1, throttle[0]);
    hw_dcMotor_setThrottle(handle->wheel_2, throttle[1]);
    hw_dcMotor_setThrottle(handle->wheel_3, throttle[2]);
    hw_dcMotor_setThrottle(handle->wheel_4, throttle[3]);
}

void app_drivetrain_stop(DT_Handle *handle) {
    hw_dcMotor_stop(handle->wheel_1);
    hw_dcMotor_stop(handle->wheel_2);
    hw_dcMotor_stop(handle->wheel_3);
    hw_dcMotor_stop(handle->wheel_4);
    handle->state = DRIVE_STOP;
}

void app_drivetrain_setDriveState(DT_Handle* handle, Drive_State state) {
    app_drivetrain_stop(handle);
    handle->state = state;
}

void app_drivetrain_brake(DT_Handle *handle) {
    Drive_State state = handle->state;
    app_drivetrain_stop(handle);

    uint8_t brakeThrot[] = {100, 100, 100, 100};

    if (state == UP) {
        app_drivetrain_drive(handle, brakeThrot, DOWN);
    } else if (state == DOWN) {
        app_drivetrain_drive(handle, brakeThrot, UP);
    } else if (state == LEFT) {
        app_drivetrain_drive(handle, brakeThrot, RIGHT);
    } else if (state == RIGHT) {
        app_drivetrain_drive(handle, brakeThrot, LEFT);
    }

    app_drivetrain_stop(handle);
}
