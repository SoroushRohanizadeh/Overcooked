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

#define BLACK_THRESHOLD 600
#define CORRECTION_FACTOR 0.25

#define BRAKE_TICKS 1

void app_lineFollowing_tick(LF_Handle* handle, uint8_t throttle, Drive_State state) {
    if (state != DRIVE_LEFT && state != DRIVE_RIGHT) return;

    io_adc_read_raw(handle->sns);
    volatile uint16_t* values = handle->sns->adcBuffer;

    uint8_t newThrottle[4] = {throttle, throttle, throttle, throttle};


    if (state == DRIVE_LEFT) {
        if (values[R1] <= BLACK_THRESHOLD && values[R2] > BLACK_THRESHOLD) {
            // go up
            newThrottle[1] = throttle * CORRECTION_FACTOR;
            newThrottle[3] = throttle * CORRECTION_FACTOR;
        } else if (values[R1] > BLACK_THRESHOLD && values[R2] <= BLACK_THRESHOLD) {
            // go down
            newThrottle[2] = throttle * CORRECTION_FACTOR;
            newThrottle[0] = throttle * CORRECTION_FACTOR;
        }
    } else {
        if (values[L1] <= BLACK_THRESHOLD && values[L2] > BLACK_THRESHOLD) {
            // go down
            newThrottle[2] = throttle * CORRECTION_FACTOR;
            newThrottle[0] = throttle * CORRECTION_FACTOR;
        } else if (values[L1] > BLACK_THRESHOLD && values[L2] <= BLACK_THRESHOLD) {
            // go up
            newThrottle[1] = throttle * CORRECTION_FACTOR;
            newThrottle[3] = throttle * CORRECTION_FACTOR;
        }
    }

    app_drivetrain_tickDrive(handle->drive, newThrottle);
}

void app_lineFollowing_tickNAVI(LF_Handle *handle, uint8_t throttle, Drive_State state, uint8_t* count, bool* stop) {
    if (*stop) return;

    io_adc_read_raw(handle->sns);
    volatile uint16_t* values = handle->sns->adcBuffer;

    char msg[16];
    if (values[T2] < BLACK_THRESHOLD) {
        app_lineFollowing_tick(handle, throttle, state);

        sprintf(msg, "%d\t Coasting\r\n", *count);
        HAL_UART_Transmit(&huart3, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
    } else if (values[T2] >= BLACK_THRESHOLD) {
        if (*count < BRAKE_TICKS) {
            uint8_t throt[] = {100,100,100,100};
            app_drivetrain_driveArray(handle->drive, throt, DRIVE_LEFT);

            sprintf(msg, "%d\t BRAKING\r\n", *count);
            HAL_UART_Transmit(&huart3, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
            (*count)++;
        } else if (*count >= BRAKE_TICKS) {
            *stop = true;
            app_drivetrain_stop(handle->drive);
        }
    }
}

bool app_lineFollowing_lineDetected(ADC_Handler* handle, uint8_t sns) {
    io_adc_read_raw(handle);
    return handle->adcBuffer[sns] >= BLACK_THRESHOLD;
}
