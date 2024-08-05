#ifndef APP_PRESTACKING_H
#define APP_PRESTACKING_H
#include <app_stacking.h>

typedef enum __PreStacking_State {
    PRE_STACKING,
    PRE_STACKING_DONE
} PreStacking_State;

#endif //APP_PRESTACKING_H

/**
 *
 * @param lastItemHeight height of last food item placed
 */
void app_preStacking_init(STACK_Handle* handle, uint16_t lastItemHeight);
void app_preStacking_tick(STACK_Handle* handle);