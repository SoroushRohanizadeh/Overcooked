#include "app_drivetrain.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define WHEEL_RADIUS 0.024 // meters
#define CLICKS_PER_REV 30 * 11 * 4
#define MAX_THROTTLE 100

void app_drivetrain_setDriveState(DT_Handle* handle, Drive_State state);

void app_drivetrain_drive(DT_Handle *handle, uint8_t throttle, Drive_State state) {
    uint8_t throt[4] = {throttle, throttle, throttle, throttle};
    app_drivetrain_driveArray(handle, throt, state);
}

void app_drivetrain_driveArray(DT_Handle *handle, uint8_t throttle[4], Drive_State state) {
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

void app_drivetrain_driveVect(DT_Handle *handle, uint8_t throttle, double_t theta, double_t rotation) {
    double x = throttle * cos(theta);
    double y = throttle * sin(theta);

    // Calculate wheel speeds
    double front_left = y + x + rotation;
    double front_right = y - x - rotation;
    double rear_left = y - x + rotation;
    double rear_right = y + x - rotation;

    // Normalize wheel speeds if necessary
    double max_wheel_speed = fmax(fmax(fabs(front_left), fabs(front_right)), fmax(fabs(rear_left), fabs(rear_right)));
    front_left = front_left / max_wheel_speed * MAX_THROTTLE;
    front_right = front_right / max_wheel_speed * MAX_THROTTLE;
    rear_left = rear_left / max_wheel_speed * MAX_THROTTLE;
    rear_right = rear_right / max_wheel_speed * MAX_THROTTLE;

    handle->__vectThrottle[3] = -front_left;
    handle->__vectThrottle[2] = -front_right;
    handle->__vectThrottle[1] = rear_left;
    handle->__vectThrottle[0] = rear_right;

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
        app_drivetrain_driveArray(handle, brakeThrot, DOWN);
    } else if (state == DOWN) {
        app_drivetrain_driveArray(handle, brakeThrot, UP);
    } else if (state == LEFT) {
        app_drivetrain_driveArray(handle, brakeThrot, RIGHT);
    } else if (state == RIGHT) {
        app_drivetrain_driveArray(handle, brakeThrot, LEFT);
    }

    app_drivetrain_stop(handle);
}
