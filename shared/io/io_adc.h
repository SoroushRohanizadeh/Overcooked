#ifndef IO_ADC_H
#define IO_ADC_H
#include <stdbool.h>
#include <stm32f1xx_hal_adc.h>

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

#endif //IO_ADC_H
io_adc_init(ADC_HandleTypeDef* hadcs, bool numhadc, enum ADC_Pins* adcPins, uint8_t numPins);

io_adc_read(enum ADC_Pins pin);
