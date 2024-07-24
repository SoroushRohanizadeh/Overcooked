#include "io_adc.h"

ADC_HandleTypeDef hadc_1;
ADC_HandleTypeDef hadc_2;

ADC_Handler *handler;

bool io_adc_pinEnabled(enum ADC_Pins adcPin);

void io_adc_init(ADC_Handler* handle) {
    hadc_1 = handle->hadcs[0];
    if (handle->twoADC) {
        hadc_2 = handle->hadcs[1];
    }
    handler = handle;
}

void io_adc_read_raw() {
    handler->ADC_Start(&hadc1, (uint32_t*)handler->adcBuffer, handler->numPins);
    while (!*handler->adcConvCMPLT) {}
    *handler->adcConvCMPLT = 0;
}