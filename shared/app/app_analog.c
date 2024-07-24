#include "app_analog.h"

ADC_Handler *handle;
enum ADC_Pin *pins;

bool app_analog_pinEnabled(enum ADC_Pin adcPin);
uint8_t app_analog_getPinIndex(enum ADC_Pin pin);

void app_analog_init(ADC_Handler *handler, enum ADC_Pin *adcPins) {
    io_adc_init(handler);
    pins = adcPins;
    handle = handler;
}

uint16_t app_analog_readPin(enum ADC_Pin pin) {
    if (!app_analog_pinEnabled(pin)) return -1;

    io_adc_read_raw();
    return handle->adcBuffer[app_analog_getPinIndex(pin)];
}

uint8_t app_analog_getPinIndex(enum ADC_Pin pin) {
    for (int i = 0; i < handle->numPins; i++) {
        if (pins[i] == pin) {
            return i;
        }
    }
    return -1; // should never get here
}

bool app_analog_pinEnabled(enum ADC_Pin adcPin) {
    for (int i = 0; i < handle->numPins; i++) {
        if (pins[i] == adcPin) {
            return true;
        }
    }
    return false;
}