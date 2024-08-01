#include "app_lineFollowing.h"

#include <stdio.h>
#include <string.h>
#include <usart.h>

#define B1 0
#define B2 1
#define T1 2
#define T2 3
#define L1 4
#define L2 5
#define R1 6
#define R2 7

#define BLACK_THRESHOLD 400
#define CORRECTION_FACTOR 0.9

#define BRAKE_TICKS 20

void app_lineFollowing_tickPID(LF_Handle* handle, uint8_t throttle, Drive_State state) {
    if (state != LEFT && state != RIGHT) return;

    io_adc_read_raw(handle->sns);
    uint16_t* values = handle->sns->adcBuffer;

    uint8_t pidThrottle[4] = {throttle, throttle, throttle, throttle};


    if (state == RIGHT) {
        if (values[R1] <= BLACK_THRESHOLD && values[R2] > BLACK_THRESHOLD) {
            // go up
            pidThrottle[1] = throttle * (1 - CORRECTION_FACTOR);
            pidThrottle[3] = throttle * (1 - CORRECTION_FACTOR);
        } else if (values[R1] > BLACK_THRESHOLD && values[R2] <= BLACK_THRESHOLD) {
            // go down
            pidThrottle[2] = throttle * (1 - CORRECTION_FACTOR);
            pidThrottle[0] = throttle * (1 - CORRECTION_FACTOR);
        }
    } else {
        if (values[L1] <= BLACK_THRESHOLD && values[L2] > BLACK_THRESHOLD) {
            // go down
            pidThrottle[2] = throttle * (1 - CORRECTION_FACTOR);
            pidThrottle[0] = throttle * (1 - CORRECTION_FACTOR);
        } else if (values[L1] > BLACK_THRESHOLD && values[L2] <= BLACK_THRESHOLD) {
            // go up
            pidThrottle[1] = throttle * (1 - CORRECTION_FACTOR);
            pidThrottle[3] = throttle * (1 - CORRECTION_FACTOR);
        }
    }

    app_drivetrain_tickThrottle(handle->drive, pidThrottle);
}

void app_lineFollowing_tickNAVI(LF_Handle *handle, uint8_t throttle, bool *toggled, Drive_State state, uint8_t* count, bool* stop) {
    io_adc_read_raw(handle->sns);
    volatile uint16_t* values = handle->sns->adcBuffer;

    if (!*toggled) {
        app_lineFollowing_tickPID(handle, throttle, state);
        if (values[T2] >= BLACK_THRESHOLD) {
            *toggled = true;
        }
    } else {
        if (values[T2] < BLACK_THRESHOLD) {
            app_drivetrain_stop(handle->drive);
            if (*count < BRAKE_TICKS) {
                uint8_t throt[] = {100,100,100,100};
                app_drivetrain_drive(handle->drive, throt, LEFT);
                char msg[15];
                sprintf(msg, "%d\t BRAKING\r\n", *count);
                HAL_UART_Transmit(&huart3, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
            } else if (*count >= BRAKE_TICKS) {
                *stop = true;
            }
        }
    }
}
