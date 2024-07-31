#include "app_drivetrain.h"

#include <math.h>

#define MAX(x,y) (x) > (y) ? (x) : (y)

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

void app_drivetrain_driveVect(DT_Handle *handle, uint8_t throttle, uint16_t theta) {
    double sinVal = sin(theta - M_PI_4);
    double cosVal = cos(theta - M_PI_4);
    double max = MAX(sinVal, cosVal);

    hw_dcMotor_setThrottle(handle->wheel_1, (cosVal / max) * (double) throttle);
    hw_dcMotor_setThrottle(handle->wheel_2, (sinVal / max) * (double) throttle);
    hw_dcMotor_setThrottle(handle->wheel_3, (sinVal / max) * (double) throttle);
    hw_dcMotor_setThrottle(handle->wheel_4, (cosVal / max) * (double) throttle);
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
