#ifndef IO_ADC_H
#define IO_ADC_H
#endif //IO_ADC_H

#include <stdbool.h>
#include <adc.h>

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

typedef struct {
    ADC_HandleTypeDef* hadcs;
    bool twoADC;
    uint8_t numPins;
    volatile uint16_t* adcBuffer;
    bool* adcConvCMPLT;
    enum ADC_Pin *adcPins;

    HAL_StatusTypeDef (*ADC_Start)(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
} ADC_Handler;

uint16_t io_adc_readPin(ADC_Handler* handler, enum ADC_Pin pin);
