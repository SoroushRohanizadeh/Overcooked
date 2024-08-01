#include "hw_rotaryEncoder.h"

#define MAX(x,y) (x) > (y) ? (x) : (y)

void hw_rotaryEncoder_incrementCW(Rotary_Handle *handle) {
    handle->countCW++;
}

void hw_rotaryEncoder_incrementCCW(Rotary_Handle *handle) {
    handle->countCCW++;
}

void hw_rotaryEncoder_resetCountCW(Rotary_Handle *handle) {
    handle->countCW = 0;
}

void hw_rotaryEncoder_resetCountCCW(Rotary_Handle *handle) {
    handle->countCCW = 0;
}

void hw_rotaryEncoder_resetCount(Rotary_Handle *handle) {
    hw_rotaryEncoder_resetCountCW(handle);
    hw_rotaryEncoder_resetCountCCW(handle);
}

uint32_t hw_rotaryEncoder_getCount(Rotary_Handle *handle) {
    return MAX(handle->countCW, handle->countCCW);
}
