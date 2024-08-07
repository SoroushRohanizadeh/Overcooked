#include "hw_reflectance.h"


bool hw_reflectance_lineDetected(ADC_Handler* handle, uint8_t sns) {
    return handle->adcBuffer[sns] >= BLACK_THRESHOLD;
}