#include "io_adc.h"

void io_adc_read_raw(ADC_Handler* handler);
bool io_adc_pinEnabled(ADC_Handler *handler, enum ADC_Pin adcPin);
uint8_t io_adc_getPinIndex(ADC_Handler *handler, enum ADC_Pin pin);

uint16_t io_adc_readPin(ADC_Handler *handler, enum ADC_Pin pin) {
    if (!io_adc_pinEnabled(handler, pin)) return -1;

    io_adc_read_raw(handler);
    return handler->adcBuffer[io_adc_getPinIndex(handler, pin)];
}

uint8_t io_adc_getPinIndex(ADC_Handler *handler, enum ADC_Pin pin) {
    for (int i = 0; i < handler->numPins; i++) {
        if (handler->adcPins[i] == pin) {
            return i;
        }
    }
    return -1; // should never get here
}

bool io_adc_pinEnabled(ADC_Handler *handler, enum ADC_Pin adcPin) {
    for (int i = 0; i < handler->numPins; i++) {
        if (handler->adcPins[i] == adcPin) {
            return true;
        }
    }
    return false;
}
void io_adc_read_raw(ADC_Handler* handler) {
    handler->ADC_Start(&hadc1, (uint32_t*)handler->adcBuffer, handler->numPins);
    while (!*handler->adcConvCMPLT) {}
    *handler->adcConvCMPLT = 0;
}