#ifndef APP_NAVISTATEMACHINE_H
#define APP_NAVISTATEMACHINE_H
#include <app_navi.h>

#endif //APP_NAVISTATEMACHINE_H

typedef enum __NAVI_STATE_NAME {
    // NAVI
    DRIVE_VERT,
    DRIVE_HOR,
    ALIGN_VERT,
    ALIGN_HOR,
    ROTATE
} NAVI_STATE_NAME;

typedef struct __NAVI_State {
    NAVI_STATE_NAME name;

    void (*run_on_entry)();
    void (*run_on_100Hz)();
    void (*run_on_exit)();
} NAVI_State;

void app_naviStateMachine_init(NAVI_Handle* handle, const NAVI_State* start_state);

void app_naviStateMachine_setNextState(NAVI_Handle* handle, const NAVI_State* state);

void app_naviStateMachine_tick100Hz(NAVI_Handle* handle);