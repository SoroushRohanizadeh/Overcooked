#include "hw_rotaryEncoder.h"

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
