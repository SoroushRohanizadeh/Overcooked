//
// Created by rohan on 2024-07-17.
//

#include "io_adc.h"
ADC_HandleTypeDef hadc_1;
ADC_HandleTypeDef hadc_2;

enum ADC_Pins* pins;
uint8_t pinCount;


int io_adc_init(ADC_HandleTypeDef* hadcs, bool twoHandles, enum ADC_Pins *adcPins, uint8_t numPins) {
    hadc_1 = hadcs[0];
    if (twoHandles) {
        hadc_2 = hadcs[1];
    }

    pinCount = numPins;
    pins = adcPins;
}

int io_adc_read(enum ADC_Pins pin) {
}
