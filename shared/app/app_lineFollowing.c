#include "app_lineFollowing.h"

#define T1 0
#define T2 1
#define L1 2
#define L2 3
#define R2 4
#define R1 5
#define B1 6
#define B2 7

#define PROP_COEFF 1.0

void app_lineFollowing_tickPID(LF_Handle* handle, uint8_t throttle, Drive_State state) {
    if (state != LEFT && state != RIGHT) return;

    io_adc_read_raw(handle->sns);
    uint16_t* values = handle->sns->adcBuffer;
    // uint8_t leftError = values[L2] - values[L1];
    // uint8_t rightError = values[R2] - values[R1];
    uint8_t error = ((values[L2] - values[L1]) + (values[R2] - values[R1])) / 2;

    error *= PROP_COEFF;

    uint8_t pidThrottle[4] = {throttle - error, throttle + error, throttle + error, throttle - error};



    app_drivetrain_tickThrottle(handle->drive, pidThrottle);
}
