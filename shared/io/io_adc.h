#ifndef IO_ADC_H
#define IO_ADC_H
#include <stdbool.h>
#include <adc.h>


enum ADC_Pins {
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
    enum ADC_Pins* adcPins;
    uint8_t numPins;
    volatile uint16_t* adcBuffer;
    bool* adcConvCMPLT;

    HAL_StatusTypeDef (*ADC_Start)(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
} ADC_Handler;

#endif //IO_ADC_H
void io_adc_init(ADC_Handler* handle);

void io_adc_read_raw();
