#ifndef IO_ADC_H
#define IO_ADC_H
#include <stdbool.h>
#include <adc.h>

typedef struct {
    ADC_HandleTypeDef* hadcs;
    bool twoADC;
    uint8_t numPins;
    volatile uint16_t* adcBuffer;
    bool* adcConvCMPLT;

    HAL_StatusTypeDef (*ADC_Start)(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
} ADC_Handler;

#endif //IO_ADC_H
void io_adc_init(ADC_Handler* handle);

void io_adc_read_raw();
