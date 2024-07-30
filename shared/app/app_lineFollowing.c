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

#define BLACK_THRESHOLD 700
#define CORRECTION_FACTOR 0.01

void app_lineFollowing_tickPID(LF_Handle* handle, uint8_t throttle, Drive_State state) {
    if (state != LEFT && state != RIGHT) return;

    io_adc_read_raw(handle->sns);
    uint16_t* values = handle->sns->adcBuffer;
    // char msg[57];
    // sprintf(msg, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\r\n", values[0], values[1], values[2], values[3],
    //   values[4], values[5], values[6], values[7]);
    // HAL_UART_Transmit(&huart3,  (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    uint8_t pidThrottle[4] = {throttle, throttle, throttle, throttle};

     // if (values[R1] >= BLACK_THRESHOLD && values[R2] >= BLACK_THRESHOLD) {
     //    pidThrottle[2] = throttle;
     //    pidThrottle[3] = throttle;
     // } else
    if (values[R1] > BLACK_THRESHOLD && values[R2] <= BLACK_THRESHOLD) {
         pidThrottle[2] = throttle * (1 + CORRECTION_FACTOR);
         pidThrottle[3] = throttle * (1 - CORRECTION_FACTOR);
    } else if (values[R1] <= BLACK_THRESHOLD && values[R2] > BLACK_THRESHOLD) {
     pidThrottle[2] = throttle * (1 - CORRECTION_FACTOR);
     pidThrottle[3] = throttle * (1 + CORRECTION_FACTOR);
    }

    // if (values[L1] >= BLACK_THRESHOLD && values[L2] >= BLACK_THRESHOLD) {
    pidThrottle[0] = throttle;
    pidThrottle[1] = throttle;
    // } else if (values[L1] > BLACK_THRESHOLD && values[L2] <= BLACK_THRESHOLD) {
    //     pidThrottle[0] = throttle * (1 - CORRECTION_FACTOR);
    //     pidThrottle[1] = throttle * (1 + CORRECTION_FACTOR);
    // } else if (values[L1] <= BLACK_THRESHOLD && values[L2] > BLACK_THRESHOLD) {
    //     pidThrottle[0] = throttle * (1 + CORRECTION_FACTOR);
    //     pidThrottle[1] = throttle * (1 - CORRECTION_FACTOR);
    // }

    app_drivetrain_tickThrottle(handle->drive, pidThrottle);
}
