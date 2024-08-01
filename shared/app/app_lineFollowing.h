#ifndef APP_LINEFOLLOWING_H
#define APP_LINEFOLLOWING_H
#include <app_drivetrain.h>
#endif //APP_LINEFOLLOWING_H
#include <io_adc.h>


typedef struct __LF_Handle {
    ADC_Handler* sns;
    DT_Handle* drive;
} LF_Handle;

/**
 *      W2       Up       W4
 *             T1 T2
 *         L1         R1
 *         L2         R2
 *             B1 B2
 *      W1     Down       W3
 */

void app_lineFollowing_tickPID(LF_Handle* handle, uint8_t throttle, Drive_State state);

void app_lineFollowing_tickNAVI(LF_Handle* handle, uint8_t throttle, bool* toggled, Drive_State state, uint8_t* count, bool* stop);
