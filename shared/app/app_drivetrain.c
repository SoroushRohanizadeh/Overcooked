#include "app_drivetrain.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define MAX(x,y) (x) > (y) ? (x) : (y)

#define WHEEL_RADIUS 0.024 // meters
#define CLICKS_PER_REV 30 * 11 * 4

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

void app_drivetrain_driveVect(DT_Handle *handle, int throttle, double_t theta) {
    double sinVal = sin(theta - M_PI_4);
    double cosVal = cos(theta - M_PI_4);
    double max = MAX(MAX(sinVal, -sinVal), MAX(cosVal, -cosVal));

    handle->__vectThrottle[3] = (int) ((cosVal / max) * (double) throttle);
    handle->__vectThrottle[2] = (int) ((sinVal / max) * (double) throttle);
    handle->__vectThrottle[1] = (int) ((sinVal / max) * (double) throttle);
    handle->__vectThrottle[0] = (int) ((cosVal / max) * (double) throttle);

    hw_dcMotor_drive(handle->wheel_1, handle->__vectThrottle[0]);
    hw_dcMotor_drive(handle->wheel_2, handle->__vectThrottle[1]);
    hw_dcMotor_drive(handle->wheel_3, handle->__vectThrottle[2]);
    hw_dcMotor_drive(handle->wheel_4, handle->__vectThrottle[3]);
}

void app_drivetrain_tickDriveVect(DT_Handle *handle) {
    hw_dcMotor_setThrottleSigned(handle->wheel_1, handle->__vectThrottle[0]);
    hw_dcMotor_setThrottleSigned(handle->wheel_2, handle->__vectThrottle[1]);
    hw_dcMotor_setThrottleSigned(handle->wheel_3, handle->__vectThrottle[2]);
    hw_dcMotor_setThrottleSigned(handle->wheel_4, handle->__vectThrottle[3]);
}


void app_drivetrain_tickDrive(DT_Handle *handle, uint8_t throttle[4]) {
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

void app_drivetrain_distanceZero(DT_Handle *handle) {
    hw_rotaryEncoder_resetCount(handle->wheel_1->rotary_handle);
    hw_rotaryEncoder_resetCount(handle->wheel_2->rotary_handle);
    hw_rotaryEncoder_resetCount(handle->wheel_3->rotary_handle);
    hw_rotaryEncoder_resetCount(handle->wheel_4->rotary_handle);
}

void app_drivetrain_distanceTravelled(DT_Handle *handle, double* x, double* y) {
    double v_fl = hw_rotaryEncoder_getCount(handle->wheel_1->rotary_handle);
    double v_fr = hw_rotaryEncoder_getCount(handle->wheel_2->rotary_handle);
    double v_rl = hw_rotaryEncoder_getCount(handle->wheel_3->rotary_handle);
    double v_rr = hw_rotaryEncoder_getCount(handle->wheel_4->rotary_handle);

    // Calculate the robot's linear velocities in the x and y directions
    *x = (v_fl + v_fr + v_rl + v_rr) * WHEEL_RADIUS / (4.0 * CLICKS_PER_REV);
    *y = (-v_fl + v_fr + v_rl - v_rr) / 4.0;
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
