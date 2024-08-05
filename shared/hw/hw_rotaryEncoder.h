#ifndef HW_ROTARYENCODER_H
#define HW_ROTARYENCODER_H
#include <stdint.h>

typedef struct __Rotary_Handle {
    uint16_t gpioPinCW;
    uint16_t gpioPinCCW;
    uint32_t countCW;
    uint32_t countCCW;
} Rotary_Handle;

#endif //HW_ROTARYENCODER_H




void hw_rotaryEncoder_incrementCW(Rotary_Handle* handle);

void hw_rotaryEncoder_incrementCCW(Rotary_Handle* handle);

void hw_rotaryEncoder_resetCountCW(Rotary_Handle* handle);

void hw_rotaryEncoder_resetCountCCW(Rotary_Handle* handle);

void hw_rotaryEncoder_resetCount(Rotary_Handle *handle);

uint32_t hw_rotaryEncoder_getCount(Rotary_Handle *handle);