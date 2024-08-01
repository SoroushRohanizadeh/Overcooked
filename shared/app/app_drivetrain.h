#ifndef APP_DRIVETRAIN_H
#define APP_DRIVETRAIN_H
#endif //APP_DRIVETRAIN_H

#include <hw_dcMotor.h>

typedef enum __Drive_State {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DRIVE_STOP,
    ANGLE
} Drive_State;

typedef struct __DT_handle {
    Motor_Handle* wheel_1;
    Motor_Handle* wheel_2;
    Motor_Handle* wheel_3;
    Motor_Handle* wheel_4;
    Drive_State state;
} DT_Handle;

// CLOCKWISE is defined by looking at the shaft of the motor
/*
 * __________________________TOP_BENCH________________________
 *
 *                              Up
 *
 *                    <--Left-- JP --Right-->
 *
 *                             Down
 *
 * __________________________MID_BENCH________________________
 *
 *                     W2       Up       W4
 *
 *                    <--Left-- HB --Right-->
 *
 *                     W1      Down      W3
 *
 * __________________________BOT_BENCH________________________
 */

void app_drivetrain_drive(DT_Handle* handle, uint8_t throttle[4], Drive_State state);

void app_drivetrain_driveVect(DT_Handle* handle, uint8_t throttle, uint16_t theta);

void app_drivetrain_tickThrottle(DT_Handle* handle, uint8_t throttle[4]);

void app_drivetrain_stop(DT_Handle* handle);

void app_drivetrain_distanceZero(DT_Handle* handle);

void app_drivetrain_distanceTravelled(DT_Handle* handle, double* x, double* y);

void app_drivetrain_brake(DT_Handle *handle);