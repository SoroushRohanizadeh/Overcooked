#ifndef APP_PRESTACKING_H
#define APP_PRESTACKING_H
#include <app_stacking.h>

#endif //APP_PRESTACKING_H

/**
 *
 * @param lastItemHeight height of last food item placed
 */
void app_preStacking_init(AUX_Handle* handle, uint16_t lastItemHeight);
void app_preStacking_tick(AUX_Handle* handle);