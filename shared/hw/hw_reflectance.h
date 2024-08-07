#ifndef HW_REFLECTANCE_H
#define HW_REFLECTANCE_H
#include <io_adc.h>
#include <stdbool.h>
#endif //HW_REFLECTANCE_H

#define B1 0
#define B2 1
#define T1 2
#define T2 3
#define L1 4
#define L2 5
#define R1 6
#define R2 7

#define BLACK_THRESHOLD 600

bool hw_reflectance_lineDetected(ADC_Handler* handle, uint8_t sns);