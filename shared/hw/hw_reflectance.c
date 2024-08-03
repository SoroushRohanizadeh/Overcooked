#include "hw_reflectance.h"

#define BLACK_THRESHOLD 400

bool hw_reflectance_lineDetected(ADC_Handler* handle, uint8_t sns) {
    io_adc_read_raw(handle);
    return handle->adcBuffer[sns] >= BLACK_THRESHOLD;
}