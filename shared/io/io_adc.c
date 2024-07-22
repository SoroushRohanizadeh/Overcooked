//
// Created by rohan on 2024-07-17.
//

#include "io_adc.h"
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

enum ADC_Pins* pins;
uint8_t pinCount;


int io_adc_init(ADC_HandleTypeDef* hadcs, bool twoHandles, enum ADC_Pins *adcPins, uint8_t numPins) {
    hadc1 = hadcs[0];
    if (twoHandles) {
        hadc2 = hadcs[1];
    }

    pinCount = numPins;
    pins = adcPins;
}

int io_adc_read(enum ADC_Pins pin) {
}
