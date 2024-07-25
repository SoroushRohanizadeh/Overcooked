#ifndef HW_ROTARYENCODER_H
#define HW_ROTARYENCODER_H
#endif //HW_ROTARYENCODER_H

#include <stdint.h>

typedef struct __Rotary_Handle {
    uint16_t gpioPinCW;
    uint16_t gpioPinCCW;
    uint16_t countCW;
    uint16_t countCCW;
} Rotary_Handle;

void hw_rotaryEncoder_incrementCW(Rotary_Handle* handle);

void hw_rotaryEncoder_incrementCCW(Rotary_Handle* handle);

void hw_rotaryEncoder_resetCountCW(Rotary_Handle* handle);

void hw_rotaryEncoder_resetCountCCW(Rotary_Handle* handle);