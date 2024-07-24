#ifndef APP_ANALOG_H
#define APP_ANALOG_H
#endif //APP_ANALOG_H

#include "io_adc.h"

enum ADC_Pin {
    PA0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PB0,
    PB1
};

void app_analog_init(ADC_Handler* handler, enum ADC_Pin* adcPins);

uint16_t app_analog_readPin(enum ADC_Pin pin);