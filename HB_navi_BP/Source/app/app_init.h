#ifndef APP_INIT_H
#define APP_INIT_H
#include <app_navi.h>

#endif //APP_INIT_H

void app_init_init(ADC_Handler* adcHandler);

NAVI_Handle* app_init_getNaviHandle();

const Node* app_init_getNode(char name[]);