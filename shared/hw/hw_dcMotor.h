#ifndef APP_DCMOTOR_H
#define APP_DCMOTOR_H
#endif //APP_DCMOTOR_H

#include "io_pwm.h"

/**
 * @param throttle between 0 and 100
 */
void hw_dcMotor_driveCW(PWM_Handle* cw_handle, uint8_t throttle);

/**
 * @param throttle between 0 and 100
 */
void hw_dcMotor_driveCW(PWM_Handle* ccw_handle, uint8_t throttle);